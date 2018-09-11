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

#ifndef TRANSITIONLINE_H
#define TRANSITIONLINE_H

#include <QWidget>
#include "animationitem.h"
#include "widgets_global.h"

class KeyframeHandle;
class Transition;
class WIDGETSSHARED_EXPORT TransitionLine : public QWidget
{
    Q_OBJECT
public:
    TransitionLine(AnimationItem *item, QString propertyName);

    void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;

    inline QString propertyName() {return m_propertyName;}
    inline AnimationItem *item() {return m_item;}
    inline void setPlayheadPosition(int value) {m_playheadPosition = value; update();}
    inline KeyFrame *selectedFrame() {return m_selectedFrame;}
    inline void deselectFrame() {m_selectedFrame = nullptr; update();}

    void addKeyframe(KeyFrame *key);
    void setScrollValue(int value);

signals:
    void keyframeDeleted(AnimationItem *item, QString propertyName, KeyFrame *frame);
    void deleteTransition(AnimationItem *item, QString propertyName, KeyFrame *frame);
    void transitionAdded(AnimationItem *item, QString propertyName, KeyFrame *frame);
    void transitionSelected(KeyFrame *frame);

private:
    AnimationItem *m_item;
    QString m_propertyName;
    KeyFrame *m_frame;
    KeyFrame *m_selectedFrame;
    int m_oldx;
    int m_playheadPosition;
    int m_horizontalScrollValue;

private slots:
    void deleteKeyframe(KeyframeHandle *handle);
    void addTransition(KeyFrame *key);
    void moveKeyframe(KeyframeHandle *handle, int dist);
    void moveTransition(Transition *transition, int dist);
    void transitionResized();
};

#endif // TRANSITIONLINE_H
