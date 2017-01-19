#ifndef SCENE_H
#define SCENE_H

#include <QDataStream>
#include "item.h"

#define ITEM 1
#define RECTANGLE 2

class Scene
{
public:
    Scene();
    Scene(int width, int height, int fps);
    ~Scene();

    quint16 width();
    quint16 height();
    quint8 fps();
    int childCount();
    Item* childAt(int pos);

    void setWidth(quint16 width);
    void setHeight(quint16 height);
    void setFps(quint8 fps);
    void addItem(Item *item);

    QDataStream& read(QDataStream &dataStream);
    QDataStream& write(QDataStream &dataStream) const;

private:
    quint16 m_width;
    quint16 m_height;
    quint8 m_fps;
    QList<Item *> m_children;
};

QDataStream &operator<<(QDataStream &, const Scene &);
QDataStream &operator>>(QDataStream &, Scene &);

#endif // SCENE_H
