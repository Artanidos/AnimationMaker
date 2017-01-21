#ifndef TEXT_H
#define TEXT_H

#include <QString>
#include "serializeableitem.h"

class Text : public SerializeableItem
{
public:
    QDataStream& read(QDataStream &dataStream);
    QDataStream& write(QDataStream &dataStream) const;
};

#endif // TEXT_H
