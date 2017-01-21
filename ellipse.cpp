#include "ellipse.h"
#include "animationscene.h"

QDataStream& Ellipse::read(QDataStream &dataStream)
{
    //dataStream >> m_x;
    //dataStream >> m_y;
    return dataStream;
}

QDataStream& Ellipse::write(QDataStream &dataStream) const
{
    dataStream << (quint8)AnimationScene::ItemType::Ellipse;
    //dataStream << m_x;
    //dataStream << m_y;
    return dataStream;
}
