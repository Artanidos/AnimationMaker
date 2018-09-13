/****************************************************************************
** Copyright (C) 2018 Olaf Japp
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

#include "ruler.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

Ruler::Ruler(Ruler::RulerType rulerType, QWidget* parent)
    : QWidget(parent)
{
    m_rulerType = rulerType;
    m_origin = 0.;
    m_rulerUnit = 1.;
    m_rulerZoom = 1.;
    m_mouseTracking = true;
    m_drawText = false;

    setMouseTracking(true);
    QFont txtFont("Arial", 7, 20);
    setFont(txtFont);
}

void Ruler::setScaledRect(const QRectF rect)
{
    m_scaledRect = rect;
    update();
}

void Ruler::setCursorPos(const QPoint cursorPos)
{
    m_cursorPos = cursorPos;
    m_cursorPos.setX(m_cursorPos.x() - m_scaledRect.left());
    m_cursorPos.setY(m_cursorPos.y() - m_scaledRect.top());
    update();
}

void Ruler::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);
    QPen pen(Qt::gray, 1);
    painter.setPen(pen);
    QRectF rulerRect = this->rect();
    drawMeter(&painter, rulerRect, 4);

    painter.setOpacity(0.4);
    drawMousePosTick(&painter);
    painter.setOpacity(1.0);

    QPointF starPt = Horizontal == m_rulerType ? rulerRect.bottomLeft() : rulerRect.topRight();
    QPointF endPt = Horizontal == m_rulerType ? rulerRect.bottomRight() : rulerRect.bottomRight();
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(starPt,endPt);
}

void Ruler::mouseMoveEvent(QMouseEvent *event)
{
    m_cursorPos = event->pos();
    update();
    QWidget::mouseMoveEvent(event);
}

void Ruler::drawMeter(QPainter *painter, QRectF rulerRect, int div)
{
    qreal widgetWidth   = m_rulerType == Horizontal ? rulerRect.width()      : rulerRect.height();
    qreal scaledLenght  = m_rulerType == Horizontal ? m_scaledRect.width()    : m_scaledRect.height();
    qreal minScaledVal  = m_rulerType == Horizontal ? m_scaledRect.left()     : m_scaledRect.top();

    qreal a = widgetWidth/scaledLenght;
    qreal b = -(a*minScaledVal);

    qreal unit = 0.0001;
    qreal unitSize = a*unit;

    while(unitSize < div*7)
    {
        unit*=10;
        unitSize = a*unit;
    }

    for(int i = 1; i <= div; i *= 2)
    {
        if(i == div)
            m_drawText = true;
        else
            m_drawText = false;

        qreal tickFact = (qreal)i / (qreal)div;

        drawFromOrigin(painter, rulerRect, b, 0, widgetWidth, unit*tickFact, unitSize*tickFact, tickFact);
        drawFromOrigin(painter, rulerRect, b, widgetWidth, widgetWidth, unit*tickFact, unitSize*tickFact, tickFact);
    }
}

void Ruler::drawFromOrigin(QPainter *painter, QRectF rulerRect, qreal origin, qreal until, qreal widgetWidth, qreal unit, qreal unitSize, qreal tickFact)
{
    bool sens   = origin < until;
    bool isHorz = m_rulerType == Horizontal;
    int  nbreUnit = 0;

    qreal currentTickPos;

    qreal x1, x2, y1, y2;

    for(currentTickPos = origin; sens ? currentTickPos <= until : currentTickPos >= until; sens ? currentTickPos += unitSize : currentTickPos -= unitSize)
    {
        if(currentTickPos >= 0 && currentTickPos <= widgetWidth)
        {
            x1 = isHorz ? currentTickPos : rulerRect.right();
            y1 = isHorz ? rulerRect.bottom() : currentTickPos;
            x2 = isHorz ? currentTickPos : rulerRect.right() - rulerRect.right()*tickFact;
            y2 = isHorz ? rulerRect.bottom() - rulerRect.bottom()*tickFact : currentTickPos;

            painter->drawLine(x1, y1, x2, y2);

            if(m_drawText)
            {
                qreal xt = isHorz ? currentTickPos + 2 : rulerRect.left() + 2;
                qreal yt = isHorz ? rulerRect.top() + 11 : currentTickPos - 5;
                painter->drawText(xt, yt, QString::number(unit*nbreUnit));
            }
        }
        nbreUnit++;
    }
}

void Ruler::drawMousePosTick(QPainter* painter)
{
    if (m_mouseTracking)
    {
        QPoint starPt = m_cursorPos;
        QPoint endPt;
        if (Horizontal == m_rulerType)
        {
            starPt.setY(this->rect().top());
            endPt.setX(starPt.x());
            endPt.setY(this->rect().bottom());
        }
        else
        {
            starPt.setX(this->rect().left());
            endPt.setX(this->rect().right());
            endPt.setY(starPt.y());
        }
        painter->drawLine(starPt,endPt);
    }
}
