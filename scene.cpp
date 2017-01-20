#include "scene.h"
#include "rectangle.h"

Scene::Scene()
{
    m_width = 0;
    m_height = 0;
    m_fps = 0;
}

Scene::Scene(int width, int height, int fps)
{
    m_width = width;
    m_height = height;
    m_fps = fps;
}

Scene::~Scene()
{
    m_children.clear();
}

quint16 Scene::width()
{
    return m_width;
}

quint16 Scene::height()
{
    return m_height;
}

quint8 Scene::fps()
{
    return m_fps;
}

int Scene::childCount()
{
    return m_children.count();
}

Item* Scene::childAt(int pos)
{
    return m_children.at(pos);
}

void Scene::setWidth(quint16 width)
{
    m_width = width;
}

void Scene::setHeight(quint16 height)
{
    m_height = height;
}

void Scene::setFps(quint8 fps)
{
    m_fps = fps;
}

void Scene::addItem(Item *item)
{
    m_children.append(item);
}

QDataStream& Scene::read(QDataStream &dataStream)
{
    quint8 type;
    m_children.clear();
    dataStream >> m_width;
    dataStream >> m_height;
    dataStream >> m_fps;
    while (!dataStream.atEnd())
    {
        dataStream >> type;
        if(type == RECTANGLE)
        {
            Rectangle *rect = new Rectangle();
            rect->read(dataStream);
            addItem(rect);
        }
    }
    return dataStream;
}

QDataStream& Scene::write(QDataStream &dataStream) const
{
    dataStream << m_width;
    dataStream << m_height;
    dataStream << m_fps;
    for(int i=0;i<m_children.count(); i++)
    {
        Item *item = m_children.at(i);
        item->write(dataStream);
    }
    return dataStream;
}

QDataStream& operator <<(QDataStream &out, const Scene &s)
{
    return s.write(out);
}

QDataStream& operator >>(QDataStream &in, Scene &s)
{
    return s.read(in);
}
