#include "ellipse.h"
#include "scene.h"

#include <QPainter>
#include <QColor>

Ellipse::Ellipse()
    : Ellipse(0,0,0)
{
}

Ellipse::Ellipse(int x, int y, int radius)
    : Item(x,y)
{
    m_radius = radius;
}

QString Ellipse::getTypeName()
{
    return QString("Ellipse");
}

void Ellipse::setRadius(int radius)
{
    m_radius = radius;
}

void Ellipse::setBrush(QBrush brush)
{
    m_brush = brush;
}

void Ellipse::setPenBrush(QBrush brush)
{
    m_penBrush = brush;
}

void Ellipse::setPenWidth(qreal width)
{
    m_penWidth = width;
}

QDataStream& Ellipse::read(QDataStream &dataStream)
{
    dataStream >> m_x;
    dataStream >> m_y;
    dataStream >> m_radius;
    dataStream >> m_brush;
    dataStream >> m_penBrush;
    dataStream >> m_penWidth;
    return dataStream;
}

QDataStream& Ellipse::write(QDataStream &dataStream) const
{
    dataStream << (quint8)ELLIPSE;
    dataStream << m_x;
    dataStream << m_y;
    dataStream << m_radius;
    dataStream << m_brush;
    dataStream << m_penBrush;
    dataStream << m_penWidth;
    return dataStream;
}

void Ellipse::paint(QPainter *painter)
{
    painter->setPen(QPen(m_penBrush, m_penWidth));
    painter->setBrush(m_brush);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->drawEllipse(QPoint(m_x, m_y), m_radius, m_radius);
}
