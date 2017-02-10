#include "itemhandle.h"

#include <QTest>

ItemHandle::ItemHandle(QGraphicsItem *parent,  int corner) :
    QGraphicsItem(parent),
    mouseDownX(0),
    mouseDownY(0),
    m_color(Qt::black),
    m_pen(Qt::white),
    m_width(9),
    m_height(9),
    m_corner(corner),
    m_mouseButtonState(kMouseReleased)
{
    setParentItem(parent);
    m_pen.setWidth(1);
    this->setAcceptHoverEvents(true);
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
    painter->fillRect(rect,brush);
}
