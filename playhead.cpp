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

PlayHead::PlayHead(Qt::Orientation orientation)
    : QSlider(orientation)
{
    m_image = QImage(":/images/playhead.png");
}

void PlayHead::paintEvent(QPaintEvent *)
{
    QColor gray = QColor(64, 66, 68);
    int width = size().width();
    int height = size().height();
    int position = QStyle::sliderPositionFromValue(minimum(), maximum(), value(), width);

    QPainter painter(this);

    painter.setPen(QColor(41, 41, 41));
    painter.fillRect(0, 0, width, height, gray);
    painter.drawRect(0, 0, width - 1, height - 1);

    painter.drawImage(position - 4, 1, m_image);
}

void PlayHead::mouseReleaseEvent(QMouseEvent *event)
{
    int pos = QStyle::sliderValueFromPosition(minimum(), maximum(), event->x(), width(), 0);
    setValue(qRound((qreal)pos / 100) * 100);
}
