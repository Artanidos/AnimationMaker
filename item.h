#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QPainter>

class Item
{
public:
    Item(int x, int y);
    //virtual ~Item(){}
    virtual QString getTypeName();
    virtual QDataStream& write(QDataStream &dataStream) const;
    virtual void paint(QPainter *painter);
    quint16 x();
    quint16 y();
    void setX(int x);
    void setY(int y);

protected:
    quint16 m_x;
    quint16 m_y;
};

#endif // ITEM_H
