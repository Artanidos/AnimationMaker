#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include "resizeableitem.h"

class Ellipse : public QGraphicsEllipseItem
{
public:
    Ellipse(qreal width, qreal height);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    QRectF boundingRect() const;
    bool sceneEventFilter(QGraphicsItem * watched, QEvent * event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    enum { Type = UserType + 2 };
    int type() const Q_DECL_OVERRIDE;

private:
    ItemHandle*  m_handles[8];
    bool m_hasHandles;
    void setHandlePositions();
};

#endif // ELLIPSE_H
