#ifndef TIMELINE_H
#define TIMELINE_H

#include <QTreeView>
#include "timelinemodel.h"

class Timeline : public QWidget
{
public:
    Timeline();

private:
    QTreeView *m_treeview;
    TimelineModel *m_timelineModel;
};

#endif // TIMELINE_H
