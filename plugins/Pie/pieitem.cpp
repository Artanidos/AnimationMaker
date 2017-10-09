/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of FlatSiteBuilder.
**
**  FlatSiteBuilder is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  FlatSiteBuilder is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with FlatSiteBuilder.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "pieitem.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>

ResizeableItem *PieItem::getInstance(AnimationScene *scene)
{
    return new Pie(scene);
}

ResizeableItem *PieItem::getInstanceFromXml(AnimationScene *scene, QDomElement ele)
{
    Pie *first = new Pie(scene);
    first->readAttributes(ele);
    return first;
}

Pie::Pie(AnimationScene *scene)
    : ResizeableItem(scene)
{
    setRect(0, 0, 200, 200);
}

QDomElement Pie::getXml(QDomDocument doc)
{
    QDomElement ele = doc.createElement("Pie");
    writeAttributes(ele);
    return ele;
}

void Pie::paint(QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    paint->setPen(pen());
    paint->setBrush(brush());
    paint->drawEllipse(rect());

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}
