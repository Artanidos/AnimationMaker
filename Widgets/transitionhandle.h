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

#include <QWidget>

#include "widgets_global.h"

class Transition;
class KeyFrame;
class WIDGETSSHARED_EXPORT TransitionHandle : public QWidget
{
    Q_OBJECT
public:
    TransitionHandle(Transition *parent, KeyFrame *key);

    void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE = 0;
    virtual void mouseReleaseEvent(QMouseEvent *ev) Q_DECL_OVERRIDE = 0;

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

    QImage m_image;
    bool m_pressed;
    KeyFrame *m_key;
    int m_oldX;

signals:
    void keyframeMoved(int pos);
};

#endif // TRANSITIONHANDLE_H
