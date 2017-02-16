#include "transitionpanel.h"

#include <QPainter>
#include <QTest>

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
    QColor orange = QColor(255,127,42);

    int width = size().width();
    int height = size().height();
    painter.setPen(QColor(41, 41, 41));
    painter.fillRect(0, 0, width, height, QColor(64, 66, 68));
    painter.drawRect(0, 0, width-1, height-1);

    int rows = m_timelineModel->rowCount();
    int row = 0;
    for(int i = 0; i < rows; i++)
    {
        painter.fillRect(1, 1 + -14 * m_scrollPos + row * 14, 200, 14, Qt::blue);
        row++;
        if(m_treeview->isExpanded(m_timelineModel->index(i, 0)))
        {
            QModelIndex childIndex = m_timelineModel->index(i, 0);
            int children = m_timelineModel->rowCount(childIndex);
            for(int j = 0; j < children; j++)
            {
                painter.fillRect(1, 1 + -14 * m_scrollPos + row * 14, 200, 14, orange);
                row++;
            }
        }
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
