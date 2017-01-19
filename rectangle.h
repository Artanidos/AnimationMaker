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

    quint16 width();
    quint16 height();
    void setWidth(int width);
    void setHeight(int height);

private:
    quint16 m_width;
    quint16 m_height;
};

#endif // RECTANGLE_H
