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
#include <QPainter>

Transition::Transition(TransitionLine *parent, KeyFrame *key)
{
    m_pressed = false;
    m_key = key;

    setParent(parent);
    setMouseTracking(true);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    resize((m_key->next()->time() - m_key->time()) / 5, 18);
    setVisible(true);

    m_left = new TransitionHandleLeft(this, key);
    connect(m_left, SIGNAL(keyframeMoved(int)), this, SLOT(sizeTransitionLeft(int)));
    m_right = new TransitionHandleRight(this, key->next());
    connect(m_right, SIGNAL(keyframeMoved(int)), this, SLOT(sizeTransitionRight(int)));
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

void Transition::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        m_pressed = true;
        m_oldX = ev->x();
    }
}

void Transition::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_pressed)
    {
        int p = x() + ev->x() - m_oldX;
        int newVal = qRound((qreal)p * 5 / 100) * 100;
        if(newVal >= 0)
            emit transitionMoved(this, newVal);
    }
}

void Transition::mouseReleaseEvent(QMouseEvent *)
{
    if(m_pressed)
        m_pressed = false;
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
    if(width > 0 && time >= 0 && (m_key->prev() == nullptr || m_key->prev()->time() < time))
    {
        m_key->setTime(time);
        resize(width, 18);
        m_right->move(width - 5,0);
        emit transitionResized();
        emit transitionMoved(this, time);
    }
}

void Transition::sizeTransitionRight(int time)
{
    int width = (time - m_key->time()) / 5;
    if(width > 0 && (m_key->next()->next() == nullptr || m_key->next()->next()->time() > time))
    {
        m_key->next()->setTime(time);
        resize(width, 18);
        m_right->move(width - 5,0);
        emit transitionResized();
    }
}
