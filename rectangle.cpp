#include "rectangle.h"

Rectangle::Rectangle()
    : Rectangle(0,0,0,0)
{
}

Rectangle::Rectangle(int x, int y, int width, int height)
    : Item(x, y)
{
    m_width = width;
    m_height = height;
    m_brush = QColor(Qt::black);
    m_penBrush = QColor(Qt::white);
    m_penWidth =  1.0;
}

QString Rectangle::getTypeName()
{
    return QString("Rectangle");
}

QDataStream& Rectangle::read(QDataStream &dataStream)
{
    dataStream >> m_x;
    dataStream >> m_y;
    dataStream >> m_width;
    dataStream >> m_height;
    dataStream >> m_brush;
    dataStream >> m_penBrush;
    dataStream >> m_penWidth;
    return dataStream;
}

QDataStream& Rectangle::write(QDataStream &dataStream) const
{
    dataStream << (quint8)2;
    dataStream << m_x;
    dataStream << m_y;
    dataStream << m_width;
    dataStream << m_height;
    dataStream << m_brush;
    dataStream << m_penBrush;
    dataStream << m_penWidth;
    return dataStream;
}

quint16 Rectangle::width()
{
    return m_width;
}

quint16 Rectangle::height()
{
    return m_height;
}

void Rectangle::setWidth(int width)
{
    m_width = width;
}

void Rectangle::setHeight(int height)
{
    m_height = height;
}

void Rectangle::setBrush(QBrush brush)
{
    m_brush = brush;
}

void Rectangle::setPenBrush(QBrush brush)
{
    m_penBrush = brush;
}

void Rectangle::setPenWidth(qreal width)
{
    m_penWidth = width;
}

void Rectangle::paint(QPainter *painter)
{
    painter->setPen(QPen(m_penBrush, m_penWidth));
    painter->setBrush(m_brush);
    painter->drawRect(m_x, m_y, m_width, m_height);
}
