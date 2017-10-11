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

#include "bitmap.h"

#include <QtTest/QTest>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>
#include <QBuffer>

Bitmap::Bitmap()
    : AnimationItem(NULL)
{

}

Bitmap::Bitmap(QString filename, AnimationScene *scene)
    : AnimationItem(scene)
{
    m_image.load(filename);
    setRect(0, 0, m_image.width(), m_image.height());
}

Bitmap::Bitmap(QImage image, qreal width, qreal height, AnimationScene *scene)
    :AnimationItem(scene)
{
    m_image = image;
    setRect(0, 0, width, height);
}

QDomElement Bitmap::getXml(QDomDocument doc)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    getImage().save(&buffer, "PNG");
    QDomElement ele = doc.createElement("Bitmap");
    writeAttributes(ele);
    ele.appendChild(doc.createCDATASection(QString::fromLatin1(byteArray.toBase64().data())));
    return ele;
}

void Bitmap::setImage(QImage image)
{
    m_image = image;
}

int Bitmap::type() const
{
    return Bitmap::Type;
}

void Bitmap::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    paint->drawImage(0, 0, m_image.scaled(rect().width(), rect().height()));

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}

QImage Bitmap::getImage()
{
    return m_image;
}
