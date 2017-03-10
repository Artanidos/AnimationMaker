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
    inline void setPlayheadPosition(int value) {m_playhead->setValue(value);}

public slots:
    void onCustomContextMenu(const QPoint &point);
    void playAnimation();
    void revertAnimation();
    void forwardAnimation();
    void selectionChanged(const QItemSelection& current,const QItemSelection&);
    void playheadValueChanged(int val);
    void playheadMoved(int val);
    void addKeyFrame(ResizeableItem *item,QString property, qreal value);

signals:
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

    void addProperty(const QString name);
    bool eventFilter(QObject *object, QEvent *event);
    void createAnimationGroup();
};

#endif // TIMELINE_H
