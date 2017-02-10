#ifndef ITEMHANDLE_H
#define ITEMHANDLE_H


#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>

class ItemHandle : public QGraphicsItem
{
public:
    explicit ItemHandle(QGraphicsItem *parent = 0,  int corner = 0);

    int  getCorner();
    void setMouseState(int);
    int  getMouseState();

    qreal mouseDownX;
    qreal mouseDownY;

    enum {kMouseReleased=0, kMouseDown, kMouseMoving};

private:
    virtual QRectF boundingRect() const;
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent * event );
    virtual void mousePressEvent(QGraphicsSceneDragDropEvent *event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );

    QColor m_color;
    QPen m_pen;
    qreal m_width;
    qreal m_height;
    int m_corner;
    int m_mouseButtonState;
};

#endif // ITEMHANDLE_H
