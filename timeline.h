#ifndef TIMELINE_H
#define TIMELINE_H

#include <QTreeView>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include "timelinemodel.h"
#include "resizeableitem.h"
#include "animationscene.h"
#include "transitionpanel.h"

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
    void addPropertyAnimation(ResizeableItem *item, const QString propertyName, qreal value);
    void selectionChanged(const QItemSelection& current,const QItemSelection&);
    void animationAdded(ResizeableItem *item, QPropertyAnimation *anim);
    void animationChanged();

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
    QSlider *m_slider;

    void addProperty(const QString name);
};

#endif // TIMELINE_H
