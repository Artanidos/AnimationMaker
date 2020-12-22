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

#include "keyframehandle.h"

#include <QPainter>

KeyframeHandle::KeyframeHandle(TransitionLine *parent, KeyFrame *key)
{
    m_key = key;
    m_pressed = false;

    setMouseTracking(true);
    setParent(parent);
    setCursor(Qt::SizeHorCursor);
    setMaximumHeight(11);
    setMaximumWidth(11);
    setVisible(true);
    setFocusPolicy(Qt::ClickFocus);

    m_imageRaute = QImage(":/images/raute-weiss.png");
    m_imageRauteHohl = QImage(":/images/raute-hohl.png");

    m_contextMenu = new QMenu();
    m_transitionAct = new QAction("Create transition");
    m_delKeyframeAct = new QAction("Delete keyframe");

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    connect(m_transitionAct, SIGNAL(triggered(bool)), this, SLOT(addTransition()));
    connect(m_delKeyframeAct, SIGNAL(triggered(bool)), this, SLOT(deleteKeyframe()));

    setContextMenuPolicy(Qt::CustomContextMenu);
}

void KeyframeHandle::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(m_key->prev() == nullptr || m_key->prev()->value() == m_key->value())
        painter.drawImage(0, 0, m_imageRaute);
    else
        painter.drawImage(0, 0, m_imageRauteHohl);

    if(hasFocus())
    {
        painter.setPen(QColor(255, 127, 42));
        painter.drawRect(0,0, width() - 1, height() - 1);
    }
}

void KeyframeHandle::onCustomContextMenu(const QPoint &point)
{
    m_contextMenu->clear();
    if(m_key->prev() && m_key->prev()->easing() < 0)
        m_contextMenu->addAction(m_transitionAct);
    m_contextMenu->addAction(m_delKeyframeAct);
    m_contextMenu->exec(this->mapToGlobal(point));
}

void KeyframeHandle::addTransition()
{
    emit transitionAdded(m_key->prev());
}

void KeyframeHandle::deleteKeyframe()
{
    emit keyframeDeleted(this);
}

void KeyframeHandle::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        m_pressed = true;
    }
}

void KeyframeHandle::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_pressed)
    {
        int p = x() + ev->x();
        int newVal = qRound((qreal)p * 5 / 100) * 100;
        if(newVal >= 0 && (m_key->next() == nullptr || m_key->next()->time() >= newVal) && (m_key->prev() == nullptr || m_key->prev()->time() <= newVal))
        {
            move(newVal / 5 - 6, y());
        }
    }
}

void KeyframeHandle::mouseReleaseEvent(QMouseEvent *ev)
{
    if(m_pressed)
    {
        m_pressed = false;
        TransitionLine *tl = dynamic_cast<TransitionLine*>(parent());
        int p = x() + ev->x();
        int newVal = qRound((qreal)p * 5 / 100) * 100 + tl->horizontalScrollValue() * 100;
        if(newVal < 0)
            newVal = 0;
        if(m_key->next() && m_key->next()->time() < newVal)
            newVal = m_key->next()->time();
        if(m_key->prev() && m_key->prev()->time() > newVal)
            newVal = m_key->prev()->time();
        emit keyframeMoved(this, newVal);
    }
}

void KeyframeHandle::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
        case Qt::Key_Left:
            emit keyframeMoved(this, m_key->time() - 100);
            break;
        case Qt::Key_Right:
            emit keyframeMoved(this, m_key->time() + 100);
            break;
    }
}
