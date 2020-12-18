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
class QMenu;
class WIDGETSSHARED_EXPORT Transition : public QWidget
{
    Q_OBJECT
public:
    Transition(TransitionLine *parent, KeyFrame *key, Timeline *timeline, QUndoStack *undostack);

    void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void resize(int w, int h);

    KeyFrame *key() {return m_key;}
    void resizeTransition();

#ifdef TEST
    TransitionHandleLeft *getLeftHandle() {return m_left;}
    TransitionHandleRight *getRightHandle() {return m_right;}
#endif
protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

signals:
    void transitionMoved(Transition *transition, int time);
    void transitionResized();
    void transitionRemoved(KeyFrame *key);

private slots:
    void onCustomContextMenu(const QPoint &point);
    void sizeTransitionLeft(int time);
    void sizeTransitionRight(int time);
    void removeTransition();
    void addTransition(KeyFrame *key);

private:
    KeyFrame *m_key;
    bool m_pressed;
    TransitionHandleLeft *m_left;
    TransitionHandleRight *m_right;
    int m_oldX;
    QMenu *m_contextMenu;
    QAction *m_transitionAct;
    Timeline *m_timeline;
    QUndoStack *m_undostack;
    int m_newTime;

    int calculatePos(int pos, int scrollPos);
};

#endif // TRANSITION_H
