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

#ifndef TRANSITION_H
#define TRANSITION_H

#include <QWidget>
#include "widgets_global.h"

class TransitionLine;
class KeyFrame;
class WIDGETSSHARED_EXPORT Transition : public QWidget
{
    Q_OBJECT
public:
    Transition(TransitionLine *parent, KeyFrame *key);

    void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;

    KeyFrame *key() {return m_key;}

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

signals:
    void transitionMoved(Transition *transition, int dist);

private:
    QImage m_imageLeft;
    QImage m_imageRight;
    KeyFrame *m_key;
    bool m_pressed;
};

#endif // TRANSITION_H
