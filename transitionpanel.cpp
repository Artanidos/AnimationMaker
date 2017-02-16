#include "transitionpanel.h"

#include <QPainter>
#include <QTest>
#include <limits.h>

TransitionPanel::TransitionPanel()
{
    m_scrollPos = 0;
    setSizePolicy(QSizePolicy ::Expanding , QSizePolicy ::Expanding );
    setMinimumWidth(100);
    setMinimumHeight(50);
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
    for(int l = 14; l < height; l+=14)
    {
        painter.drawLine(0, l, width, l);
    }
    for(int i = 0; i < rows; i++)
    {
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
                    painter.drawRect(begin / 5 + 1, 1 + -14 * m_scrollPos + row * 14, duration / 5 - 2, 12);
                    painter.fillRect(begin / 5 + 2, 2 + -14 * m_scrollPos + row * 14, duration / 5 - 3, 11, orange2);
                    row++;
                    childsPainted++;
                }
            }
        }
        painter.setPen(QPen(gray));
        painter.fillRect(minBegin / 5 + 1, 1 + -14 * m_scrollPos + (row - childsPainted - 1) * 14 + 5, (maxTime - minBegin) / 5 - 1, 8, orange);
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
