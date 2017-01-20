#include <QtGui>
#include "editor.h"

Editor::Editor(QWidget *parent)
    : QWidget(parent)
{
}

void Editor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    for(int i=0; i<m_scene->childCount(); i++)
    {
        Item *item = m_scene->childAt(i);
        item->paint(&painter);
    }
}

QSize Editor::sizeHint() const
{
    QSize size(m_scene->width(), m_scene->height());
    return size;
}

void Editor::setScene(Scene *scene)
{
    m_scene = scene;
    resize(m_scene->width(), m_scene->height());
    update();
}
