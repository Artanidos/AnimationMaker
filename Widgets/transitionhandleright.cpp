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

#include "transitionhandleright.h"
#include "transition.h"
#include "keyframe.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

TransitionHandleRight::TransitionHandleRight(Transition *parent, KeyFrame *key)
    : TransitionHandle (parent, key)
{
    m_image = QImage(":/images/trans-right.png");
    move(parent->width() - 5,0);
}

void TransitionHandleRight::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        m_pressed = true;
        m_oldX = ev->x();
        m_oldTime = m_key->time();
    }
}

void TransitionHandleRight::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_pressed)
    {
        int p = x() + ev->x() - m_oldX;
        int newVal = m_key->prev()->time() + qRound((qreal)p * 5 / 100) * 100;
        m_key->setTime(newVal);
        Transition *transition = dynamic_cast<Transition*>(parent());
        transition->resizeTransition();
    }
}

void TransitionHandleRight::mouseReleaseEvent(QMouseEvent *ev)
{
    m_pressed = false;
    int newTime = m_key->time();
    m_key->setTime(m_oldTime);
    emit keyframeMoved(newTime);
}
