#include "rectangle.h"
#include "animationscene.h"

QDataStream& Rectangle::read(QDataStream &dataStream)
{
    //dataStream >> m_x;
    //dataStream >> m_y;
    return dataStream;
}

QDataStream& Rectangle::write(QDataStream &dataStream) const
{
    dataStream << (quint8)AnimationScene::ItemType::Rectangle;
    //dataStream << m_x;
    //dataStream << m_y;
    return dataStream;
}
