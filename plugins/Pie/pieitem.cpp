/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of AnimationMaker.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "pieitem.h"
#include "propertyeditor.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>

ResizeableItem *PieItem::getInstance(AnimationScene *scene)
{
    Pie *p = new Pie(scene, className());
    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::blue));
    return p;
}

ResizeableItem *PieItem::getInstanceFromXml(AnimationScene *scene, QDomElement ele)
{
    Pie *p = new Pie(scene, className());
    p->readAttributes(ele);
    p->setPen(QPen(QColor(ele.attribute("pen", "#000000"))));
    p->setBrush(QBrush(QColor(ele.attribute("brush", "#0000FF"))));
    p->setStartAngle(ele.attribute("startAngle", "45").toInt());
    p->setSpanAngle(ele.attribute("spanAngle", "270").toInt());
    return p;
}

Pie::Pie(AnimationScene *scene, QString typeName)
    : ResizeableItem(scene)
{
    setRect(0, 0, 200, 200);
    m_startAngle = 45;
    m_spanAngle = 270;
    m_typeName = typeName;
}

QDomElement Pie::getXml(QDomDocument doc)
{
    QDomElement ele = doc.createElement("Pie");
    writeAttributes(ele);
    ele.setAttribute("pen", pen().color().name());
    ele.setAttribute("brush", brush().color().name());
    ele.setAttribute("startAngle", m_startAngle);
    ele.setAttribute("spanAngle", m_spanAngle);
    return ele;
}

QList<PropertyEditorInterface *> *Pie::getPropertyEditors()
{
    QList<PropertyEditorInterface*> *list = new QList<PropertyEditorInterface*>;
    list->append(new PropertyEditor);
    return list;
}

void Pie::setStartAngle(int angle)
{
    m_startAngle = angle;
    update();
}

void Pie::setSpanAngle(int angle)
{
    m_spanAngle = angle;
    update();
}

void Pie::paint(QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    paint->setPen(pen());
    paint->setBrush(brush());
    paint->drawPie(rect(), m_startAngle * 16, m_spanAngle * 16);
    paint->drawText(rect(), QString::number(playheadPosition()), QTextOption(Qt::AlignCenter));

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}
