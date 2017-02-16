#ifndef TRANSITIONPANEL_H
#define TRANSITIONPANEL_H

#include <QWidget>
#include <QModelIndex>
#include <QTreeView>
#include "timelinemodel.h"

class TransitionPanel :public QWidget
{
    Q_OBJECT
public:
    TransitionPanel();

    void paintEvent(QPaintEvent*);
    void setModel(TimelineModel *model);
    void setTreeview(QTreeView *tv);

public slots:
    void treeExpanded(QModelIndex);
    void treeCollapsed(QModelIndex);
    void treeScrollValueChanged(int);
    void treeItemAdded();

private:
    int m_scrollPos;
    TimelineModel *m_timelineModel;
    QTreeView *m_treeview;
};

#endif // TRANSITIONPANEL_H
