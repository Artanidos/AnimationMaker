#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "item.h"
#include <QDataStream>

class Rectangle : public Item
{
public:
    Rectangle();
    Rectangle(int x, int y, int width, int height);

    virtual QString getTypeName();
    QDataStream& read(QDataStream &dataStream);
    QDataStream& write(QDataStream &dataStream) const;
    void paint(QPainter *painter);

    quint16 width();
    quint16 height();
    void setWidth(int width);
    void setHeight(int height);
    void setBrush(QBrush brush);
    void setPenBrush(QBrush brush);
    void setPenWidth(qreal width);

private:
    quint16 m_width;
    quint16 m_height;
    QBrush m_brush;
    QBrush m_penBrush;
    qreal m_penWidth;
};

#endif // RECTANGLE_H
