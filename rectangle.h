#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "serializeableitem.h"
#include <QDataStream>
#include <QGraphicsRectItem>

class Rectangle : public SerializeableItem, QGraphicsRectItem
{
public:
    Rectangle();

    QDataStream& read(QDataStream &dataStream);
    QDataStream& write(QDataStream &dataStream) const;
};

#endif // RECTANGLE_H
