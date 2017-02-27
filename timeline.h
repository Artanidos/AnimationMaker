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

    QParallelAnimationGroup *getAnimations();
    void reset();

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
    void deleteAnimation();

signals:
    void animationSelectionChanged(QPropertyAnimation *anim);
    void itemSelectionChanged(ResizeableItem *item);
    void itemAdded();

private:
    QTreeView *m_treeview;
    TimelineModel *m_timelineModel;
    QMenu *m_contextMenu;
    QAction *m_delAct;
    AnimationScene *m_scene;
    TransitionPanel *m_transitionPanel;
    PlayHead *m_playhead;
    QParallelAnimationGroup *m_parallelAnimations;

    void addProperty(const QString name);
    bool eventFilter(QObject *object, QEvent *event);
    void createAnimationGroup();
};

#endif // TIMELINE_H
