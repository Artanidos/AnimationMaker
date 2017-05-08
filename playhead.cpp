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
#include "timeline.h"
#include <QStyle>
#include <QPainter>
#include <QTest>

PlayHead::PlayHead()
{
    m_image = QImage(":/images/playhead.png");
    m_pressed = false;
    m_value = 0;
    m_horizontalScrollPos = 0;
    setMinimumHeight(35);
}

void PlayHead::paintEvent(QPaintEvent *)
{
    QColor gray = QColor(64, 66, 68);
    int width = size().width();
    int height = size().height();
    int offset = m_horizontalScrollPos * 20;

    QPainter painter(this);
    QFont font = painter.font();
    font.setPixelSize(10);
    painter.setFont(font);
    painter.setPen(Qt::white);
    QFontMetrics fm = painter.fontMetrics();
    for(int i = 0 - offset; i < width; i+=20)
    {
        painter.drawLine(i, 12, i, 13);
    }
    for(int i = 0 - offset; i < width; i+=100)
    {
        painter.drawLine(i, 12, i, 20);
        if(i > 0)
        {
            QString posString = timeString((i + offset) * 5, false);
            int width = fm.width(posString);
            painter.drawText(i - width / 2, 0, width, fm.height(), 0, posString);
        }
    }
    painter.setPen(QColor(41, 41, 41));
    painter.fillRect(0, 22, width, height, gray);
    painter.drawRect(0, 22, width - 1, height - 1);
    painter.drawImage(m_value / 5 - 4 - offset, 17, m_image);
}

void PlayHead::scrollValueChanged(int pos)
{
    m_horizontalScrollPos = pos;
    update();
}

void PlayHead::mousePressEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;
    if(event->x() + m_horizontalScrollPos * 20 > m_value / 5 - 4 && event->x() + m_horizontalScrollPos * 20 <= m_value / 5 + 5)
        m_pressed = true;
    else
    {
        int x = event->x();
        if(x < 0)
            x = 0;
        if(x >= width())
            x = width() - 1;
        x += m_horizontalScrollPos * 20;
        m_value = qRound((qreal)x * 5 / 100) * 100;
        m_pressed = true;
        update();
        emit valueChanged(m_value);
    }
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
        x += m_horizontalScrollPos * 20;
        m_value = qRound((qreal)x * 5 / 100) * 100;
        update();
        emit valueChanged(m_value);
    }
}

void PlayHead::mouseReleaseEvent(QMouseEvent *)
{
    m_pressed = false;
}
