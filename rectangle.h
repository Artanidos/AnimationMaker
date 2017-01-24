#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include "itemhandle.h"

class Rectangle : public QGraphicsRectItem
{
public:
    Rectangle(qreal width, qreal height);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    QRectF boundingRect() const;
    bool sceneEventFilter(QGraphicsItem * watched, QEvent * event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE;

private:
    ItemHandle*  m_handles[8];
    bool m_hasHandles;
    void setHandlePositions();
};

#endif // RECTANGLE_H
