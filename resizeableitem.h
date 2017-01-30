#ifndef RESIZEABLEITEM_H
#define RESIZEABLEITEM_H

#include <QGraphicsItem>
#include "itemhandle.h"

class ResizeableItem : public QGraphicsItem
{
public:
    ResizeableItem();

    void drawHighlightSelected(QPainter *painter, const QStyleOptionGraphicsItem *option);
    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    QRectF boundingRect() const;
    bool sceneEventFilter(QGraphicsItem * watched, QEvent * event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    QRectF rect() const;
    void setRect(qreal x, qreal y, qreal w, qreal h);

    QPen pen() const;
    void setPen(const QPen &pen);

    QBrush brush() const;
    void setBrush(const QBrush &brush);

private:
    ItemHandle*  m_handles[8];
    bool m_hasHandles;
    QRectF m_rect;
    QPen m_pen;
    QBrush m_brush;

protected:
    void setHandlePositions();
};

#endif // RESIZEABLEITEM_H
