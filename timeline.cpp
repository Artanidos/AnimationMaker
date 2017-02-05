#include "timeline.h"

#include <QHeaderView>

Timeline::Timeline()
    : QWidget(0)
{
    m_timelineModel = new TimelineModel();
    m_treeview = new QTreeView(this);
    m_treeview->setModel(m_timelineModel);
    m_treeview->header()->close();
}
