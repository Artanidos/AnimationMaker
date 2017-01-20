#include "item.h"
#include "scene.h"

Item::Item(int x, int y)
{
    m_x = x;
    m_y = y;
}

quint16 Item::x()
{
    return m_x;
}

quint16 Item::y()
{
    return m_y;
}

void Item::setX(int x)
{
    m_x = x;
}

void Item::setY(int y)
{
    m_y = y;
}

QString Item::getTypeName()
{
    return QString("Item");
}

QDataStream& Item::write(QDataStream &dataStream) const
{
    dataStream << (quint8)ITEM;
    dataStream << m_x;
    dataStream << m_y;
    return dataStream;
}

void Item::paint(QPainter *painter)
{

}
