#ifndef TIMELINE_H
#define TIMELINE_H

#include <QTreeView>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include "timelinemodel.h"
#include "resizeableitem.h"

class Timeline : public QWidget
{
    Q_OBJECT
public:
    Timeline();

    void addItemToAnimate(QGraphicsItem *item);

public slots:
    void addXProperty();
    void addYProperty();
    void addOpacityProperty();
    void onCustomContextMenu(const QPoint &point);
    void playAnimation();
    void selectionChanged(const QItemSelection&, const QItemSelection&);

signals:
    void playAnimationPressed();
    void animationSelectionChanged(QPropertyAnimation *anim);
    void itemSelectionChanged(ResizeableItem *item);

private:
    QTreeView *m_treeview;
    TimelineModel *m_timelineModel;
    QMenu *m_contextMenu;
    QMenu *m_propertiesMenu;
    QAction *m_xAct;
    QAction *m_yAct;
    QAction *m_opacityAct;

    void addProperty(const QString name);
};

#endif // TIMELINE_H
