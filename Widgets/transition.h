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
class TransitionHandleLeft;
class TransitionHandleRight;
class Timeline;
class QUndoStack;
class WIDGETSSHARED_EXPORT Transition : public QWidget
{
    Q_OBJECT
public:
    Transition(TransitionLine *parent, KeyFrame *key, Timeline *timeline, QUndoStack *undostack);

    void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;

    KeyFrame *key() {return m_key;}
    void resizeTransition();

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

signals:
    void transitionMoved(Transition *transition, int time);
    void transitionResized();

private slots:
    void sizeTransitionLeft(int time);
    void sizeTransitionRight(int time);

private:
    KeyFrame *m_key;
    bool m_pressed;
    TransitionHandleLeft *m_left;
    TransitionHandleRight *m_right;
    int m_oldX;
    Timeline *m_timeline;
    QUndoStack *m_undostack;
};

#endif // TRANSITION_H
