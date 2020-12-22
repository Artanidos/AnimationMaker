/****************************************************************************
** Copyright (C) 2016 Olaf Japp
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

#include "ellipse.h"

#include <QString>

Ellipse::Ellipse(AnimationScene *scene)
    : AnimationItem(scene)
{
    setRect(0, 0, 0, 0);
}

QDomElement Ellipse::getXml(QDomDocument doc)
{
    QDomElement ele = doc.createElement("Ellipse");
    writeAttributes(ele);
    ele.setAttribute("pen", pen().color().name());
    ele.setAttribute("brush", brush().color().name());
    return ele;
}

int Ellipse::type() const
{
    return Ellipse::Type;
}

void Ellipse::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    paint->setPen(pen());
    paint->setBrush(brush());
    paint->drawEllipse(rect());

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}

QString Ellipse::getHtml(QString id, QString)
{
    QString html = "<svg width=\"" + QString::number(width()) + "\" height=\"" + QString::number(height()) + "\">\n";
    html += "<ellipse ";
    html += "id=\"" + id + "\" ";
    html += "cx=\"" + QString::number((qreal)width() / 2.0) + "\" ";
    html += "cy=\"" + QString::number((qreal)height() / 2.0) + "\" ";
    html += "rx=\"" + QString::number((qreal)width() / 2.0) + "\" ";
    html += "ry=\"" + QString::number((qreal)height() / 2.0) + "\" ";
    html += "fill=\"" + brushColor().name() + "\" ";
    html += "stroke=\"" + penColor().name() + "\" ";
    html += "stroke-width=\"1\" ";
    html += "/>\n";
    html += "</svg>\n";
    return html;
}
