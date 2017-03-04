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

#include "text.h"

#include <QTest>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>

Text::Text(QString text)
    : ResizeableItem()
{
   m_font = QFont("Arial", 13);
   m_text = text;
   m_textcolor = QColor(Qt::black);
   QFontMetrics m(m_font);
   setRect(0, 0, m.width(m_text), m.height());
   m_textitem = new QGraphicsSimpleTextItem(m_text, this);
   m_textitem->setFont(m_font);
   m_textitem->setBrush(QBrush(m_textcolor));
}

int Text::type() const
{
    return Text::Type;
}

void Text::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}

QString Text::text()
{
    return m_text;
}

void Text::setText(QString text)
{
    m_text = text;
    m_textitem->setText(text);
    QFontMetrics m(m_font);
    setRect(0, 0, m.width(m_text) * m_xscale, m.height() * m_yscale);
    setHandlePositions();
}

QColor Text::textcolor()
{
    return m_textcolor;
}

void Text::setTextcolor(QColor color)
{
   m_textcolor = color;
   m_textitem->setBrush(QBrush(m_textcolor));
}

void Text::setScale(qreal x, qreal y)
{
    m_xscale = x;
    m_yscale = y;
    QTransform trans;
    trans.scale(m_xscale, m_yscale);
    m_textitem->setTransform(trans);

    QFontMetrics m(m_font);
    setRect(0, 0, m.width(m_text) * m_xscale, m.height() * m_yscale);
}

void Text::scaleObjects()
{
    QFontMetrics m(m_font);
    m_xscale = rect().width() / m.width(m_text);
    m_yscale = rect().height() / m.height();

    QTransform trans;
    trans.scale(m_xscale, m_yscale);
    m_textitem->setTransform(trans);
}
