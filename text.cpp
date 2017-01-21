#include "text.h"

#include "animationscene.h"

QDataStream& Text::read(QDataStream &dataStream)
{
    //dataStream >> m_x;
    //dataStream >> m_y;
    return dataStream;
}

QDataStream& Text::write(QDataStream &dataStream) const
{
    dataStream << (quint8)AnimationScene::ItemType::Text;
    //dataStream << m_x;
    //dataStream << m_y;
    return dataStream;
}

