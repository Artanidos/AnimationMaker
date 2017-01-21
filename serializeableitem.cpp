#include "serializeableitem.h"

QDataStream& SerializeableItem::write(QDataStream &dataStream) const
{
    return dataStream;
}
