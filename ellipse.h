#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "item.h"

class Ellipse : public Item
{
public:
    Ellipse();
    Ellipse(int x, int y, int radius);

    virtual QString getTypeName();
    QDataStream& read(QDataStream &dataStream);
    QDataStream& write(QDataStream &dataStream) const;
    void paint(QPainter *painter);

    void setRadius(int radius);
    void setBrush(QBrush brush);
    void setPenBrush(QBrush brush);
    void setPenWidth(qreal width);

private:
    quint16 m_radius;
    QBrush m_brush;
    QBrush m_penBrush;
    qreal m_penWidth;
};

#endif // ELLIPSE_H
