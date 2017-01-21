#ifndef SERIALIZEABLEITEM_H
#define SERIALIZEABLEITEM_H

#include <QDataStream>

class SerializeableItem
{
public:
    virtual QDataStream& write(QDataStream &dataStream) const;
};

#endif // SERIALIZEABLEITEM_H
