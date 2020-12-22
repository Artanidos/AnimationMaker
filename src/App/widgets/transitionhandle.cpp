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

#include "transitionhandle.h"

#include <QColor>
#include <QPainter>

#include "transition.h"

TransitionHandle::TransitionHandle(Transition *parent, KeyFrame *key)
{
    m_pressed = false;
    m_key = key;

    setMouseTracking(true);
    setParent(parent);
    setVisible(true);
    setCursor(Qt::SizeHorCursor);
    setMaximumHeight(15);
    setMaximumWidth(4);
    setFocusPolicy(Qt::ClickFocus);
}

void TransitionHandle::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0, 2, m_image);

    if(hasFocus())
    {
        painter.setPen(QColor(255, 127, 42));
        painter.drawRect(0,0, width() - 1, height() - 1);
    }
}

void TransitionHandle::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
        case Qt::Key_Left:
            emit keyframeMoved(m_key->time() - 100);
            break;
        case Qt::Key_Right:
            emit keyframeMoved(m_key->time() + 100);
            break;
    }
}
