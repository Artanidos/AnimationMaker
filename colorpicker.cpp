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

#include "colorpicker.h"

#include <QPainter>
#include <QTest>

ColorPicker::ColorPicker()
{
    m_hue = 0.0;
    m_lpressed = false;
    setMinimumSize(100, 100);
}

void ColorPicker::setHue(qreal value)
{
    m_hue = value;
    update();
}

qreal ColorPicker::hue()
{
   return m_hue;
}

QSize ColorPicker::sizeHint() const
{
    return QSize(100, 100);
}

void ColorPicker::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    qreal s, l, a;

    QImage image(100, 100, QImage::Format_ARGB32);
    for(int x=0; x < 100; x++)
    {
        for(int y=0;y < 100; y++)
        {
            s = x / 100.0;
            l = (100 - y) / 100.0;
            a = 1.0;
            QColor color = QColor::fromHslF(m_hue, s, l, a);
            image.setPixel(x, y, color.rgba());
        }
    }
    painter.drawImage(0, 0, image);
}

void ColorPicker::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_lpressed = true;
        emit colorChanged(getColor(event));
    }
}

QColor ColorPicker::getColor(QMouseEvent *event)
{
    qreal s, l, a;
    int x, y;

    x = event->pos().x();
    y = event->pos().y();
    if(x < 0)
        x = 0;
    if(x > 100)
        x = 100;
    if(y < 0)
        y = 0;
    if(y > 100)
        y = 100;
    s = x / 100.0;
    l = (100 - y) / 100.0;
    a = 1.0;
    return QColor::fromHslF(m_hue, s, l, a);
}

void ColorPicker::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && m_lpressed)
    {
        emit colorChanged(getColor(event));
    }
}
