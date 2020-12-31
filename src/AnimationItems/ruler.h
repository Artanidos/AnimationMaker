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

#ifndef RULER_H
#define RULER_H

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QRectF>
#include <QWidget>

class Ruler : public QWidget
{
    Q_OBJECT
public:
    enum RulerType { Horizontal, Vertical };

    Ruler(Ruler::RulerType rulerType, QWidget* parent = nullptr);

public slots:
    void setScaledRect      (const QRectF rect);
    void setCursorPos       (const QPoint cursorPos);

protected:
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    RulerType m_rulerType;
    qreal m_rulerUnit;
    qreal m_rulerZoom;
    qreal m_origin;
    bool m_drawText;
    QPoint m_cursorPos;
    bool m_mouseTracking;
    QRectF m_scaledRect;


    void drawMousePosTick(QPainter* painter);
    void drawMeter(QPainter *painter, QRectF rulerRect, int div);
    void drawFromOrigin(QPainter *painter, QRectF rulerRect, qreal origin, qreal until, qreal widgetWidth, qreal unit, qreal unitSize, qreal tickFact);
    //void drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin);
    //void drawFromOriginTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition);
};

#endif // RULER_H
