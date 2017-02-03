#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include "resizeableitem.h"

class Rectangle : public ResizeableItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
public:
    Rectangle(qreal width, qreal height, QMenu *menu);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);

    enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE;
};

#endif // RECTANGLE_H
