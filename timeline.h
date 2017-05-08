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

#include <QTreeWidget>
#include <QGraphicsItem>
#include <QLabel>
#include <QToolButton>
#include <QPropertyAnimation>
#include "resizeableitem.h"
#include "animationscene.h"
#include "transitionpanel.h"
#include "playhead.h"

class Timeline : public QWidget
{
    Q_OBJECT
public:
    Timeline(AnimationScene *scene);

    void reset();
    inline void setPlayheadPosition(int value) {m_playhead->setValue(value);}
    inline void expandTree() {m_tree->expandAll();}
    int lastKeyframe();
    void removeItem(ResizeableItem *item);
    void selectItem(ResizeableItem *item);
    void addKeyFrame(ResizeableItem *item, QString propertyName, KeyFrame *frame);
    void deleteKeyFrame(ResizeableItem *item, QString propertyName, KeyFrame *frame);
    void addTransition(ResizeableItem *item, QString propertyName, KeyFrame *frame);
    void deleteTransition(ResizeableItem *item, QString propertyName, KeyFrame *frame);

public slots:
    void onCustomContextMenu(const QPoint &point);
    void playAnimation();
    void pauseAnimation();
    void revertAnimation();
    void forwardAnimation();
    void playheadValueChanged(int val);
    void addKeyFrame(ResizeableItem *item,QString property, QVariant value);
    void autokeyframes(bool value);
    void autotransitions(bool value);
    void idChanged(ResizeableItem *item, QString value);
    void treeCurrentItemChanged(QTreeWidgetItem *currentItem, QTreeWidgetItem*);
    void keyframeAdded(ResizeableItem * item, QString propertyName, KeyFrame *key);
    void deleteKeyFrameSlot(ResizeableItem *item, QString propertyName, KeyFrame *frame);
    void deleteTransitionSlot(ResizeableItem *item, QString propertyName, KeyFrame *frame);
    void addTransitionSlot(ResizeableItem *item, QString propertyName, KeyFrame *frame);
    void transitionSelected(KeyFrame *frame);
    void transitionDeselected();
    void scrollValueChanged(int value);

signals:
    void itemSelectionChanged(ResizeableItem *item);
    void keyframeAdded(ResizeableItem *item, QString propertyName);
    void keyframeDeleted(ResizeableItem *item, QString propertyName);
    void transitionDeleted(ResizeableItem *item, QString propertyName);
    void transitionSelectionChanged(KeyFrame *frame);

private:
    QScrollBar *m_sb;
    QTreeWidget *m_tree;
    QMenu *m_contextMenu;
    QAction *m_delAct;
    AnimationScene *m_scene;
    TransitionPanel *m_transitionPanel;
    PlayHead *m_playhead;
    QLabel *m_time;
    bool m_playing;
    QToolButton *playButton;
    QToolButton *pauseButton;

    void addProperty(const QString name);
    void createAnimationGroup();
    QTreeWidgetItem *search(ResizeableItem *item);
    QTreeWidgetItem *search(QTreeWidgetItem *treeItem, QString propertyName);
};
QString timeString(int milliseconds, bool showMinutes = true);
#endif // TIMELINE_H
