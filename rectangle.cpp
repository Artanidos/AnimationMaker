#include "rectangle.h"

Rectangle::Rectangle()
    : Item(0, 0)
{
    m_width = 0;
    m_height = 0;
}

Rectangle::Rectangle(int x, int y, int width, int height)
    : Item(x, y)
{
    m_width = width;
    m_height = height;
}

QString Rectangle::getTypeName()
{
    return QString("Rectangle");
}

QDataStream& Rectangle::read(QDataStream &dataStream)
{
    dataStream >> m_width;
    dataStream >> m_height;
    return dataStream;
}

QDataStream& Rectangle::write(QDataStream &dataStream) const
{
    dataStream << (quint8)2;
    dataStream << m_width;
    dataStream << m_height;
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
