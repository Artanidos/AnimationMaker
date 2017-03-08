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

#include <QPainter>
#include <QTest>
#include <limits.h>

TransitionPanel::TransitionPanel()
{
    m_scrollPos = 0;
    setSizePolicy(QSizePolicy ::Expanding , QSizePolicy ::Expanding );
    setMinimumWidth(100);
    setMinimumHeight(50);

    m_imageRaute = QImage(":/images/raute-weiss.png");
    m_imageRauteHohl = QImage(":/images/raute-hohl.png");
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
    QColor orange = QColor(255,127,42);
    QColor orange2 = QColor(255,127,42, 150);
    int row = 0;
    int width = size().width();
    int height = size().height();
    int rows = m_timelineModel->rowCount();

    painter.setPen(QColor(41, 41, 41));
    painter.fillRect(0, 0, width, height, gray);
    painter.drawRect(0, 0, width-1, height-1);
    painter.setPen(QColor(41, 41, 41));
    for(int k = 200; k < width; k+=200)
    {
        painter.drawLine(k, 0, k, height);
    }
    for(int l = 15; l < height; l+=15)
    {
        painter.drawLine(0, l, width, l);
    }
    for(int i = 0; i < rows; i++)
    {
        row++;
        QModelIndex childIndex = m_timelineModel->index(i, 0);
        int children = m_timelineModel->rowCount(childIndex);
        for(int j = 0; j < children; j++)
        {
            QModelIndex keyframeIndex = m_timelineModel->index(j, 0, childIndex);
            QVariant var = m_timelineModel->data(keyframeIndex, Qt::UserRole);

            QList<KeyFrame*> *list = (QList<KeyFrame*> *) var.value<void *>();
            if(list)
            {
                for(int k=0; k < list->count(); k++)
                {
                    KeyFrame *frame = list->at(k);
                    if(m_treeview->isExpanded(m_timelineModel->index(i, 0)))
                    {
                        painter.drawImage(frame->time() / 5 - 3, 1 + -15 * m_scrollPos + row * 15, m_imageRaute);
                    }
                }
            }
            if(m_treeview->isExpanded(m_timelineModel->index(i, 0)))
                row++;
        }
        /*
        row++;
        QModelIndex childIndex = m_timelineModel->index(i, 0);
        int children = m_timelineModel->rowCount(childIndex);
        int minBegin = std::numeric_limits<int>::max();
        int maxTime = 0;
        int childsPainted = 0;
        for(int j = 0; j < children; j++)
        {
            QModelIndex animationIndex = m_timelineModel->index(j, 0, childIndex);
            QVariant var = m_timelineModel->data(animationIndex, Qt::UserRole);

            QPropertyAnimation *anim = (QPropertyAnimation *) var.value<void *>();
            if(anim)
            {
                int begin = anim->property("begin").toInt();
                minBegin = minBegin < begin ?  minBegin : begin;
                int duration = anim->duration();
                maxTime = maxTime > (duration + begin) ? maxTime : (duration + begin);
                if(m_treeview->isExpanded(m_timelineModel->index(i, 0)))
                {
                    painter.setPen(QPen(orange));
                    painter.drawRect(begin / 5 + 1, 1 + -15 * m_scrollPos + row * 15, duration / 5 - 2, 13);
                    painter.fillRect(begin / 5 + 2, 2 + -15 * m_scrollPos + row * 15, duration / 5 - 3, 12, orange2);
                    row++;
                    childsPainted++;
                }
            }

        }
        painter.setPen(QPen(gray));
        painter.fillRect(minBegin / 5 + 1, 1 + -15 * m_scrollPos + (row - childsPainted - 1) * 15 + 5, (maxTime - minBegin) / 5 - 1, 8, orange);
        */
    }
}

void TransitionPanel::treeExpanded(QModelIndex)
{
    update();
}

void TransitionPanel::treeCollapsed(QModelIndex)
{
    update();
}

void TransitionPanel::treeScrollValueChanged(int to)
{
    m_scrollPos = to;
    update();
}

void TransitionPanel::treeItemAdded()
{
    update();
}
