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

#include "transition.h"
#include "transitionline.h"
#include "transitionhandleleft.h"
#include "transitionhandleright.h"
#include "commands.h"
#include <QPainter>
#include <QMenu>


Transition::Transition(TransitionLine *parent, KeyFrame *key, Timeline *timeline, QUndoStack *undostack)
{
    m_pressed = false;
    m_key = key;
    m_timeline = timeline;
    m_undostack = undostack;

    setParent(parent);
    setMouseTracking(true);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    m_contextMenu = new QMenu();
    m_transitionAct = new QAction("Remove transition");
    m_contextMenu->addAction(m_transitionAct);

    m_left = new TransitionHandleLeft(this, key);
    connect(m_left, SIGNAL(keyframeMoved(int)), this, SLOT(sizeTransitionLeft(int)));
    connect(m_left, SIGNAL(transitionAdded(KeyFrame*)), this, SLOT(addTransition(KeyFrame*)));
    m_right = new TransitionHandleRight(this, key->next());
    connect(m_right, SIGNAL(keyframeMoved(int)), this, SLOT(sizeTransitionRight(int)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    connect(m_transitionAct, SIGNAL(triggered(bool)), this, SLOT(removeTransition()));

    setContextMenuPolicy(Qt::CustomContextMenu);
    resize((m_key->next()->time() - m_key->time()) / 5, 18);
    setVisible(true);
}

void Transition::paintEvent(QPaintEvent *)
{
    QColor orange(255, 127, 42, 150);
    QColor orangeSelected(255, 127, 42, 255);

    int width = size().width();
    int height = size().height();

    QPainter painter(this);
    if(hasFocus())
        painter.fillRect(0, 0, width, height, orangeSelected);
    else
        painter.fillRect(0, 0, width, height, orange);
}

void Transition::onCustomContextMenu(const QPoint &point)
{
    m_contextMenu->exec(this->mapToGlobal(point));
}

void Transition::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        m_pressed = true;
        m_oldX = ev->x();
        m_newTime = m_key->time();
    }
}

void Transition::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_pressed)
    {
        TransitionLine *tl = dynamic_cast<TransitionLine*>(parent());
        m_newTime = calculatePos(ev->x());
        if(m_key->prev() && m_key->prev()->easing() > -1)
        {
            QList<Transition*> transitions = tl->findChildren<Transition*>();
            foreach(Transition *trans, transitions)
            {
                if(trans->key() == m_key->prev())
                {
                    int width = (m_newTime - m_key->prev()->time()) / 5;
                    trans->resize(width, 18);
                }
            }
        }
        if(m_key->next()->easing() > -1)
        {
            QList<Transition*> transitions = tl->findChildren<Transition*>();
            foreach(Transition *trans, transitions)
            {
                if(trans->key() == m_key->next())
                {
                    int width = (m_key->next()->next()->time() - (m_key->next()->time() - m_key->time()) - m_newTime) / 5;
                    trans->resize(width, 18);
                    trans->move((m_newTime + m_key->next()->time() - m_key->time()) / 5 - tl->horizontalScrollValue() * 20, 0);
                }
            }
        }
        move(m_newTime / 5 - tl->horizontalScrollValue() * 20, 0);
    }
}

void Transition::mouseReleaseEvent(QMouseEvent *ev)
{
    if(m_pressed)
    {
        m_pressed = false;
        if(m_newTime != m_key->time())
            emit transitionMoved(this, m_newTime);
    }
}

void Transition::resize(int w, int h)
{
    QWidget::resize(w, h);
    if(m_right)
        m_right->move(width() - 5, 0);
}

int Transition::calculatePos(int pos)
{
    int p = x() + pos - m_oldX;
    int newVal = qRound((qreal)p * 5 / 100) * 100;
    if(newVal < 0)
        newVal = 0;
    if(m_key->next()->next())
    {
        if(m_key->next()->easing() > -1 && m_key->next()->next()->time() - 100 < newVal + m_key->next()->time() - m_key->time())
            newVal = m_key->next()->next()->time() - (m_key->next()->time() - m_key->time()) - 100;
        else if(m_key->next()->next()->time() < newVal + m_key->next()->time() - m_key->time())
            newVal = m_key->next()->next()->time() - (m_key->next()->time() - m_key->time());
    }
    if(m_key->prev())
    {
        if(m_key->prev()->easing() > -1 && m_key->prev()->time() + 100 > newVal)
            newVal = m_key->prev()->time() + 100;
        else if(m_key->prev()->time() > newVal)
            newVal = m_key->prev()->time();
    }
    return newVal;
}

void Transition::resizeTransition()
{
    TransitionLine *tl = dynamic_cast<TransitionLine*>(parent());
    int width = (m_key->next()->time() - m_key->time()) / 5;
    resize(width, 18);
    move(m_key->time() / 5 - tl->horizontalScrollValue() * 20,0);
}

void Transition::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
        case Qt::Key_Left:
            if(m_key->time() > 99)
                emit transitionMoved(this, m_key->time() - 100);
            break;
        case Qt::Key_Right:
            emit transitionMoved(this, m_key->time() + 100);
            break;
    }
}

void Transition::sizeTransitionLeft(int time)
{
    int width = (m_key->next()->time() - time) / 5;
    if(width > 0 && time >= 0 && (m_key->prev() == nullptr || (m_key->prev()->easing() < 0 && m_key->prev()->time() <= time) || (m_key->prev()->easing() > -1 && m_key->prev()->time() < time)))
    {
        QUndoCommand *cmd = new ResizeTransitionCommand(m_key, m_key->time(), time, m_key->next()->time(), m_key->next()->time(), m_timeline);
        m_undostack->push(cmd);
    }
}

void Transition::sizeTransitionRight(int time)
{
    int width = (time - m_key->time()) / 5;
    if(width > 0 && (m_key->next()->next() == nullptr || (m_key->next()->easing() < 0 && m_key->next()->next()->time() >= time) || (m_key->next()->easing() > -1 && m_key->next()->next()->time() > time)))
    {
        QUndoCommand *cmd = new ResizeTransitionCommand(m_key, m_key->time(), m_key->time(), m_key->next()->time(), time, m_timeline);
        m_undostack->push(cmd);
    }
}

void Transition::removeTransition()
{
    TransitionLine *tl = dynamic_cast<TransitionLine*>(parent());
    m_timeline->deleteTransitionSlot(tl->item(), tl->propertyName(), m_key);
}

void Transition::addTransition(KeyFrame *key)
{
    TransitionLine *tl = dynamic_cast<TransitionLine*>(parent());
    tl->addTransition(key);
}
