#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include "resizeableitem.h"

class Rectangle : public ResizeableItem
{
public:
    Rectangle(qreal width, qreal height);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);

    enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE;
};

#endif // RECTANGLE_H
