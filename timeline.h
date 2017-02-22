#ifndef TIMELINE_H
#define TIMELINE_H

#include <QTreeView>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include "timelinemodel.h"
#include "resizeableitem.h"
#include "animationscene.h"
#include "transitionpanel.h"
#include "playhead.h"

class Timeline : public QWidget
{
    Q_OBJECT
public:
    Timeline(AnimationScene *scene);

public slots:
    void onCustomContextMenu(const QPoint &point);
    void playAnimation();
    void revertAnimation();
    void forwardAnimation();
    void addPropertyAnimation(ResizeableItem *item, const QString propertyName, qreal value, int min, int max);
    void selectionChanged(const QItemSelection& current,const QItemSelection&);
    void animationAdded(ResizeableItem *item, QPropertyAnimation *anim);
    void animationChanged();
    void playheadValueChanged(int val);

signals:
    void animationSelectionChanged(QPropertyAnimation *anim);
    void itemSelectionChanged(ResizeableItem *item);
    void itemAdded();

private:
    QTreeView *m_treeview;
    TimelineModel *m_timelineModel;
    QMenu *m_contextMenu;
    QMenu *m_propertiesMenu;
    QAction *m_xAct;
    QAction *m_yAct;
    QAction *m_opacityAct;
    AnimationScene *m_scene;
    TransitionPanel *m_transitionPanel;
    PlayHead *m_playhead;

    void addProperty(const QString name);
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // TIMELINE_H
