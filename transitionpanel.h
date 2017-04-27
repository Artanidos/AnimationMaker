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

#ifndef TRANSITIONPANEL_H
#define TRANSITIONPANEL_H

#include <QWidget>
#include <QModelIndex>
#include <QTreeWidget>
#include <QVBoxLayout>
#include "transitionline.h"

class Timeline;
class TransitionPanel : public QWidget
{
    Q_OBJECT
public:
    TransitionPanel();

    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent *);
    void setTreeWidget(QTreeWidget *tv);
    void reset();
    void setPlayheadPosition(int value);
    void removeItem(ResizeableItem *item);
    inline void registerTimeline(Timeline *timeline) {m_timeline = timeline;}
    void transitionAdded(ResizeableItem *item, QString propertyName);
    void transitionDeleted(ResizeableItem *item, QString propertyName);

public slots:
    void treeExpanded(QModelIndex);
    void treeCollapsed(QModelIndex);
    void treeScrollValueChanged(int);
    void keyframeAdded(ResizeableItem *item, QString propertyName);
    void keyframeDeleted(ResizeableItem *item, QString propertyName);
    void scrollValueChanged(int pos);
    void transitionSelected(KeyFrame *frame);

private:
    int m_verticalScrollPos;
    int m_horizontalScrollPos;
    QTreeWidget *m_tree;
    QVBoxLayout *m_layout;
    Timeline *m_timeline;
    int m_playheadPosition;

    void enableDisableLines();
    TransitionLine *getTransitionLine(ResizeableItem *item, QString propertyName);
};

#endif // TRANSITIONPANEL_H
