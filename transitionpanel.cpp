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

#include <QPainter>
#include <QTest>
#include <limits.h>
#include <QSplitter>
#include <QLayout>

TransitionPanel::TransitionPanel()
{
    m_scrollPos = 0;
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

void TransitionPanel::setModel(TimelineModel *model)
{
    m_timelineModel = model;
}

void TransitionPanel::setTreeview(QTreeView *tv)
{
    m_treeview = tv;
}

void TransitionPanel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor gray = QColor(64, 66, 68);

    int width = size().width();
    int height = size().height();

    painter.setPen(QColor(41, 41, 41));
    painter.fillRect(0, 0, width, height, gray);
    painter.drawRect(0, 0, width-1, height-1);
    painter.setPen(QColor(41, 41, 41));
    for(int k = 200; k < width; k+=200)
    {
        painter.drawLine(k, 0, k, height);
    }

    painter.setPen(Qt::red);
    painter.drawLine(m_playheadPosition / 5, 1, m_playheadPosition / 5, height - 1);
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
    int rows = m_timelineModel->rowCount();
    for(int i = 0; i < rows; i++)
    {
        QModelIndex childIndex = m_timelineModel->index(i, 0);
        QVariant v = m_timelineModel->data(childIndex, Qt::UserRole);
        ResizeableItem *item = (ResizeableItem *) v.value<void *>();
        TransitionLine *line = getTransitionLine(item, "");
        if(line)
        {
            if(m_scrollPos <= row)
                line->setVisible(true);
            row++;
            if((row + 1 - m_scrollPos) * 15 > height())
                return;
        }
        if(m_treeview->isExpanded(m_timelineModel->index(i, 0)))
        {
            for(int j = 0; j < m_timelineModel->rowCount(childIndex); j++)
            {
                QModelIndex keyframeIndex = m_timelineModel->index(j, 0, childIndex);
                TransitionLine *line = getTransitionLine(item, m_timelineModel->data(keyframeIndex, Qt::DisplayRole).toString());
                if(line)
                {
                    if(m_scrollPos <= row)
                        line->setVisible(true);
                    row++;
                    if((row + 1 - m_scrollPos) * 15 > height())
                        return;
                }
            }
        }
    }
}

TransitionLine *TransitionPanel::getTransitionLine(ResizeableItem *item, QString propertyName)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_layout->itemAt(i)->widget());
        if(line && line->propertyName() == propertyName && line->item() == item)
            return line;
    }
    return NULL;
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
    m_scrollPos = to;
    enableDisableLines();
}

void TransitionPanel::keyframeAdded(ResizeableItem *item, QString propertyName)
{
    if(item)
    {
        TransitionLine *line = new TransitionLine(item, propertyName);
        m_layout->insertWidget(m_layout->count() - 1, line);
        enableDisableLines();
    }
    else
        update();
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
