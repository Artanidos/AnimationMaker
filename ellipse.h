#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QGraphicsItem>
#include "resizeableitem.h"

class Ellipse : public ResizeableItem
{
public:
    Ellipse(qreal width, qreal height);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);

    enum { Type = UserType + 2 };
    int type() const Q_DECL_OVERRIDE;
};

#endif // ELLIPSE_H
