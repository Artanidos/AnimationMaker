/****************************************************************************
** Copyright (C) 2016 Olaf Japp
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

#include "itemhandle.h"
#include "animationitem.h"
#include <QTest>

ItemHandle::ItemHandle(AnimationItem *parent, int corner, int scaling) :
    QGraphicsItem(parent),
    mouseDownX(0),
    mouseDownY(0),
    m_color(Qt::black),
    m_pen(Qt::white),
    m_corner(corner),
    m_mouseButtonState(kMouseReleased)
{
    m_pen.setWidth(1);
    switch(scaling)
    {
    case 0:
        m_width = 18.;
        m_height = 18.;
        break;
    case 1:
        m_width = 9.;
        m_height = 9.;
        break;
    case 2:
        m_width = 4.5;
        m_height = 4.5;
        break;
    case 3:
        m_width = 2.25;
        m_height = 2.25;
        break;
    case 4:
        m_width = 2.25;
        m_height = 2.25;
        break;
    }
    this->setAcceptHoverEvents(true);
    setZValue(100);

    switch(corner)
    {
        case 0:
        case 2:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case 1:
        case 3:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case 4:
        case 6:
            setCursor(Qt::SizeVerCursor);
            break;
        case 5:
        case 7:
            setCursor(Qt::SizeHorCursor);
            break;
    }
}

void ItemHandle::setMouseState(int s)
{
    m_mouseButtonState = s;
}

int ItemHandle::getMouseState()
{
    return m_mouseButtonState;
}

int ItemHandle::getCorner()
{
    return m_corner;
}

void ItemHandle::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void ItemHandle::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void ItemHandle::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
}

void ItemHandle::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(false);
}

void ItemHandle::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(false);
}

void ItemHandle::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    m_color = Qt::black;
    this->update(0, 0, m_width, m_height);
}

void ItemHandle::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    m_color = QColor(255, 127, 42);
    this->update(0, 0, m_width, m_height);
}

QRectF ItemHandle::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void ItemHandle::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    m_pen.setCapStyle(Qt::SquareCap);
    m_pen.setStyle(Qt::SolidLine);
    painter->setPen(m_pen);
    QPointF topLeft (0, 0);
    QPointF bottomRight (m_width, m_height);
    QRectF rect (topLeft, bottomRight);
    QBrush brush (Qt::SolidPattern);
    brush.setColor(m_color);
    painter->drawRect(rect);
    painter->fillRect(rect,brush);
}
