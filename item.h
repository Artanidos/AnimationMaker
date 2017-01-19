#ifndef ITEM_H
#define ITEM_H

#include <QString>

class Item
{
public:
    Item(int x, int y);
    //virtual ~Item(){}
    virtual QString getTypeName();
    virtual QDataStream& write(QDataStream &dataStream) const;

    quint16 x();
    quint16 y();
    void setX(int x);
    void setY(int y);

private:
    quint16 m_x;
    quint16 m_y;
};

#endif // ITEM_H
