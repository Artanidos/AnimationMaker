/****************************************************************************
** Copyright (C) 2016 Olaf Japp
**
** This file is part of AnimationMaker.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "transitionpanel.h"
#include "keyframe.h"
#include "transitionline.h"
#include "timeline.h"
#include "transition.h"
#include "keyframehandle.h"
#include <QPainter>
#include <QTest>
#include <limits.h>
#include <QSplitter>
#include <QLayout>
#include <QUndoStack>

TransitionPanel::TransitionPanel(QUndoStack *undostack)
{
    m_verticalScrollPos = 0;
    m_horizontalScrollPos = 0;
    m_undostack = undostack;

    setSizePolicy(QSizePolicy ::Expanding , QSizePolicy ::Expanding );
    setMinimumWidth(100);
    setMinimumHeight(50);

    m_layout = new QVBoxLayout();
    m_layout->setMargin(1);
    m_layout->setSpacing(1);
    m_layout->addStretch();
    setLayout(m_layout);

    m_playheadPosition = 0;
}

void TransitionPanel::setTreeWidget(QTreeWidget *tv)
{
    m_tree = tv;
}

void TransitionPanel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor gray = QColor(64, 66, 68);

    int width = size().width();
    int height = size().height();
    int offset = m_horizontalScrollPos * 20;

    painter.setPen(QColor(41, 41, 41));
    painter.fillRect(0, 0, width, height, gray);
    painter.drawRect(0, 0, width-1, height-1);
    painter.setPen(QColor(41, 41, 41));
    for(int k = 200 - offset; k < width; k+=200)
    {
        painter.drawLine(k, 0, k, height);
    }

    painter.setPen(Qt::red);
    painter.drawLine(m_playheadPosition / 5 - offset, 1, m_playheadPosition / 5 - offset, height - 1);
}

void TransitionPanel::reset()
{
    QLayoutItem *item;
    while((item = m_layout->takeAt(0)))
    {
        if(item->widget())
            delete item->widget();
        delete item;
    }
    m_layout->addStretch();
}

void TransitionPanel::enableDisableLines()
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line)
            line->setVisible(false);
    }

    int row = 0;
    int rows = m_tree->topLevelItemCount();
    for(int i = 0; i < rows; i++)
    {
        QTreeWidgetItem *treeItem = m_tree->topLevelItem(i);
        AnimationItem *item = (AnimationItem *) treeItem->data(0, 1).value<void *>();
        TransitionLine *line = getTransitionLine(item, "");
        if(line)
        {
            if(m_verticalScrollPos <= row)
                line->setVisible(true);
            row++;
            if((row + 1 - m_verticalScrollPos) * 18 > height())
                return;
        }
        if(treeItem->isExpanded())
        {
            for(int j = 0; j < treeItem->childCount(); j++)
            {
                QTreeWidgetItem *childItem = treeItem->child(j);
                TransitionLine *line = getTransitionLine(item, childItem->text(0));
                if(line)
                {
                    if(m_verticalScrollPos <= row)
                        line->setVisible(true);
                    row++;
                    if((row + 1 - m_verticalScrollPos) * 18 > height())
                        return;
                }
            }
        }
    }
}

TransitionLine *TransitionPanel::getTransitionLine(AnimationItem *item, QString propertyName)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line && line->propertyName() == propertyName && line->item() == item)
            return line;
    }
    return nullptr;
}

void TransitionPanel::treeExpanded(QModelIndex)
{
    enableDisableLines();
}

void TransitionPanel::treeCollapsed(QModelIndex)
{
    enableDisableLines();
}

void TransitionPanel::treeScrollValueChanged(int to)
{
    m_verticalScrollPos = to;
    enableDisableLines();
}

void TransitionPanel::scrollValueChanged(int pos)
{
   m_horizontalScrollPos = pos;
   for(int i = 0; i < m_layout->count(); i++)
   {
       TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
       if(line)
           line->setScrollValue(pos);
   }
   update();
}

void TransitionPanel::lineAdded(AnimationItem *item)
{
    TransitionLine *line = new TransitionLine(item, "", m_timeline, m_undostack);
    line->setScrollValue(m_horizontalScrollPos);
    m_layout->insertWidget(m_layout->count() - 1, line);
    enableDisableLines();
}

void TransitionPanel::propertyAdded(AnimationItem *item, QString propertyName)
{
    TransitionLine *line = new TransitionLine(item, propertyName, m_timeline, m_undostack);
    line->setScrollValue(m_horizontalScrollPos);
    m_layout->insertWidget(m_layout->count() - 1, line);
    connect(line, SIGNAL(keyframeDeleted(AnimationItem*,QString,KeyFrame*)), m_timeline, SLOT(deleteKeyFrameSlot(AnimationItem*,QString,KeyFrame*)));
    connect(line, SIGNAL(deleteTransition(AnimationItem*,QString,KeyFrame*)), m_timeline, SLOT(deleteTransitionSlot(AnimationItem*,QString,KeyFrame*)));
    connect(line, SIGNAL(transitionAdded(AnimationItem*,QString,KeyFrame*)), m_timeline, SLOT(addTransitionSlot(AnimationItem*,QString,KeyFrame*)));
    connect(line, SIGNAL(transitionSelected(KeyFrame*)), m_timeline, SLOT(transitionSelected(KeyFrame*)));
    connect(line, SIGNAL(transitionSelected(KeyFrame*)), this, SLOT(transitionSelected(KeyFrame*)));
    enableDisableLines();
}

void TransitionPanel::propertyKeyAdded(AnimationItem *item, QString propertyName, KeyFrame *key)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line && line->propertyName() == propertyName && line->item() == item)
        {
            line->addKeyframe(propertyName, key);
            break;
        }
    }
}

void TransitionPanel::transitionSelected(KeyFrame *frame)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line && line->selectedFrame() != frame)
        {
            line->deselectFrame();
        }
    }
}

void TransitionPanel::transitionAdded(AnimationItem *item, QString propertyName, KeyFrame *key)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line && line->propertyName() == propertyName && line->item() == item)
        {
            line->update();
            break;
        }
    }
}


void TransitionPanel::transitionDeleted(AnimationItem *item, QString propertyName, KeyFrame *key)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line && line->propertyName() == propertyName && line->item() == item)
        {
            line->update();
            break;
        }
    }
}

void TransitionPanel::keyframeMoved(KeyFrame *key)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line)
        {
            KeyframeHandle *handle = line->getKeyframeHandle(key);
            if(handle)
            {
                handle->move(handle->key()->time() / 5 - line->horizontalScrollValue() * 20 - 6, 2);
                break;
            }
        }
    }
}

void TransitionPanel::transitionMoved(KeyFrame *key)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line)
        {
            Transition *transition = line->getTransition(key);
            if(transition)
            {
                transition->move(transition->key()->time() / 5 - line->horizontalScrollValue() * 20,0);
                break;
            }
        }
    }
}

void TransitionPanel::transitionResized(KeyFrame *key)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line)
        {
            Transition *transition = line->getTransition(key);
            if(transition)
            {
                transition->resizeTransition();
                break;
            }
        }
    }
}

void TransitionPanel::addTransition(AnimationItem *item, QString propertyName, KeyFrame *key)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line && line->propertyName() == propertyName && line->item() == item)
        {
            line->addTransitionGui(key);
            break;
        }
    }
}

void TransitionPanel::deleteTransition(AnimationItem *item, QString propertyName, KeyFrame *key)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line && line->propertyName() == propertyName && line->item() == item)
        {
            line->removeTransition(propertyName, key);
            break;
        }
    }
}

void TransitionPanel::deleteKeyframe(AnimationItem *item, QString propertyName)
{
    int count = 0;
    if(!item->keyframes()->value(propertyName))
    {
        for(int j = 0; j < m_layout->count(); j++)
        {
            TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(j)->widget());
            if(line && line->item() == item)
            {
                count ++;
            }
        }
        for(int i = m_layout->count() - 1; i >= 0 ; i--)
        {
            TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
            if(line)
            {
                if(line->propertyName() == propertyName && line->item() == item)
                {
                    m_layout->removeWidget(line);
                    line->setVisible(false);
                    if(count == 2) // that means we also should drop the empty line which belongs to the item
                    {
                        // if previous line has no propertyName
                        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i - 1)->widget());
                        if(line)
                        {
                            if(line->propertyName() == "" && line->item() == item)
                            {
                                m_layout->removeWidget(line);
                                line->setVisible(false);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

void TransitionPanel::resizeEvent(QResizeEvent *)
{
    enableDisableLines();
}

void TransitionPanel::setPlayheadPosition(int value)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line)
            line->setPlayheadPosition(value);
    }
    m_playheadPosition = value;
    update();
}

void TransitionPanel::removeItem(AnimationItem *item)
{
    for(int i = m_layout->count() - 1; i >= 0; i--)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line && line->item() == item)
        {
            m_layout->removeWidget(line);
            delete line;
        }
    }
}
