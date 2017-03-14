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

#include "playhead.h"
#include <QStyle>
#include <QPainter>
#include <QTest>

PlayHead::PlayHead()
{
    m_image = QImage(":/images/playhead.png");
    m_pressed = false;
    m_value = 0;
}

void PlayHead::paintEvent(QPaintEvent *)
{
    QColor gray = QColor(64, 66, 68);
    int width = size().width();
    int height = size().height();

    QPainter painter(this);

    painter.setPen(Qt::white);
    for(int i = 0; i < width; i+=20)
    {
        painter.drawLine(i, 0, i, 3);
    }
    for(int i = 0; i < width; i+=100)
    {
        painter.drawLine(i, 0, i, 8);
    }
    painter.setPen(QColor(41, 41, 41));
    painter.fillRect(0, 10, width, height, gray);
    painter.drawRect(0, 10, width - 1, height - 1);
    painter.drawImage(m_value / 5 - 4, 4, m_image);
}

void PlayHead::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && event->x() > m_value / 5 - 4 && event->x() <= m_value / 5 + 5)
        m_pressed = true;
}

void PlayHead::mouseMoveEvent(QMouseEvent *event)
{
    if(m_pressed)
    {
        int x = event->x();
        if(x < 0)
            x = 0;
        if(x >= width())
            x = width() - 1;
        m_value = qRound((qreal)x * 5 / 100) * 100;
        if(m_value / 5 > width())
            m_value -= 100;
        update();
        emit valueChanged(m_value);
    }
}

void PlayHead::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressed = false;
}
