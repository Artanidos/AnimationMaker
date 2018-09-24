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

#include "transitionhandleleft.h"
#include "transition.h"
#include "keyframe.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QMenu>

TransitionHandleLeft::TransitionHandleLeft(Transition *parent, KeyFrame *key)
    : TransitionHandle(parent, key)
{
    m_image = QImage(":/images/trans-left.png");
    m_contextMenu = new QMenu();
    m_transitionAct = new QAction("Create transition");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    connect(m_transitionAct, SIGNAL(triggered(bool)), this, SLOT(addTransition()));
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void TransitionHandleLeft::onCustomContextMenu(const QPoint &point)
{
    m_contextMenu->clear();
    if(m_key->prev() && m_key->prev()->easing() < 0)
    {
        m_contextMenu->addAction(m_transitionAct);
        m_contextMenu->exec(this->mapToGlobal(point));
    }
}

void TransitionHandleLeft::addTransition()
{
    emit transitionAdded(m_key->prev());
}

void TransitionHandleLeft::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        m_pressed = true;
        m_oldX = ev->x();
        m_oldTime = m_key->time();
    }
}

void TransitionHandleLeft::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_pressed)
    {
        int p = x() + ev->x() - m_oldX;
        int newVal = m_key->time() + qRound((qreal)p * 5 / 100) * 100;
        if(newVal >= 0 && (m_key->prev() == nullptr || m_key->prev()->time() <= newVal) && newVal < m_key->next()->time())
        {
            m_key->setTime(newVal);
            Transition *transition = dynamic_cast<Transition*>(parent());
            transition->resizeTransition();
        }
    }
}

void TransitionHandleLeft::mouseReleaseEvent(QMouseEvent *ev)
{
    if(m_pressed)
    {
        m_pressed = false;
        int newTime = m_key->time();
        m_key->setTime(m_oldTime);
        if(m_oldTime <= newTime)
            emit keyframeMoved(newTime);
    }
}
