/****************************************************************************
** Copyright (C) 2018 Olaf Japp
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

#ifndef TRANSITIONHANDLE_H
#define TRANSITIONHANDLE_H

#include <QImage>
#include <QPaintEvent>
#include <QWidget>

#include "keyframe.h"

// pre declare Transition here to avoid circular dependencies
class Transition;

class TransitionHandle : public QWidget
{
    Q_OBJECT
public:
    TransitionHandle(Transition *parent, KeyFrame *key);

    void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

    QImage m_image;
    bool m_pressed;
    KeyFrame *m_key;
    int m_oldX;
    int m_oldTime;

signals:
    void keyframeMoved(int pos);
};

#endif // TRANSITIONHANDLE_H
