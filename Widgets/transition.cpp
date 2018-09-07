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
#include <QPainter>

Transition::Transition(TransitionLine *parent, KeyFrame *key)
{
    m_pressed = false;
    m_key = key;

    setParent(parent);
    setMouseTracking(true);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    setMaximumHeight(18);
    setMinimumHeight(18);
    setMaximumWidth((m_key->next()->time() - m_key->time()) / 5);
    setMinimumWidth((m_key->next()->time() - m_key->time()) / 5);
    setVisible(true);

    m_imageLeft = QImage(":/images/trans-left.png");
    m_imageRight = QImage(":/images/trans-right.png");
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
    painter.drawImage(0, 2, m_imageLeft);
    painter.drawImage(width - 5, 2, m_imageRight);
}

void Transition::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
        case Qt::Key_Left:
            emit transitionMoved(this, -1);
            break;
        case Qt::Key_Right:
            emit transitionMoved(this, 1);
            break;
    }
}
