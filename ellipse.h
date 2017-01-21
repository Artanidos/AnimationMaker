#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "serializeableitem.h"

class Ellipse : public SerializeableItem
{
public:
    QDataStream& read(QDataStream &dataStream);
    QDataStream& write(QDataStream &dataStream) const;
};

#endif // ELLIPSE_H
