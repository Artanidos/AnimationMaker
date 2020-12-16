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

/****************************************************************************
** Just a small example how to implement nice plugins.
** The pie has been rendered into a canvas when exporting to HTML.
** So I just ported the paint method to javascript and canvas.
**
** What are you going to create?
****************************************************************************/

#include "pieitem.h"
#include "propertyeditor.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>

AnimationItem *PieItem::getInstance(AnimationScene *scene)
{
    Pie *p = new Pie(scene, className());
    p->setPen(QPen(Qt::black));
    p->setBrush(QBrush(Qt::blue));
    return p;
}

AnimationItem *PieItem::getInstanceFromXml(AnimationScene *scene, QDomElement ele)
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
    : AnimationItem(scene)
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

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}

QString Pie::getHtml(QString id, QString)
{
    // This is everything but complete.
    // For example the fillstyle and the angles are not animated yet.
    // But this sample is just an idea for you to create your own plugin.

    qreal angleA = 2.0 / 360.0 * (360 - startAngle());
    qreal angleB = 2.0 / 360.0 * (360 - spanAngle() - startAngle());

    QString html = "<canvas id=\"" + id +"\" width=\"" + QString::number(width()) + "\" height=\"" + QString::number(height()) + "\"></canvas>\n";
    html += "<script>\n";
    html += "var c = document.getElementById(\"" + id + "\");\n";
    html += "var ctx = c.getContext(\"2d\");\n";
    html += "ctx.fillStyle = \"" + brush().color().name() + "\";\n";
    html += "ctx.lineWidth = 1;\n";
    html += "ctx.beginPath();\n";
    html += "ctx.moveTo(" + QString::number(width() / 2.0) + "," + QString::number(height() / 2.0) + ");\n";
    html += "ctx.arc(" + QString::number(width() / 2.0) + ", " + QString::number(height() / 2.0) + ", " + QString::number(height() / 2.0) + ", " + QString::number(angleB) + " * Math.PI, " + QString::number(angleA) + " * Math.PI);\n";
    html += "ctx.lineTo(" + QString::number(width() / 2.0) + "," + QString::number(height() / 2.0) + ");\n";
    html += "ctx.stroke();\n";
    html += "ctx.fill();\n";
    html += "</script>\n";
    return html;
}
