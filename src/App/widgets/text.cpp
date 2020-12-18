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

#include <QFontMetrics>
#include <QTransform>
#include <QVariant>

QString Text::getHtml(QString id, QString)
{
    QString html = "<svg width=\"" + QString::number(width()) + "\" height=\"" + QString::number(height()) + "\">" + getTextTag(id) + "<svg/>";
    return html;
}

QString Text::getTextTag(QString id)
{
    QFontMetrics fm(m_font);
    QString svg = "<text ";
    if(!id.isEmpty())
        svg += "id=\"" + id + "\" ";
    svg += "x=\"0\" y=\"" + QString::number(fm.ascent()) + "\" ";
    svg += "font-family=\"" + m_font.family() + "\" ";
    svg += "font-size=\"" + QString::number((double)m_font.pointSize() * 1.25) + "px\" ";
    if(m_font.bold())
        svg += "font-weight=\"bold\" ";
    if(m_font.italic())
        svg += "font-style=\"italic\" ";
    svg += "fill=\"" + m_textcolor.name() + "\" ";
    svg += "opacity=\"" + QString::number((qreal)opacity() / 100.0) + "\" ";
    svg += ">";
    svg += m_text;
    svg += "</text>";
    return svg;
}

QString Text::getSvg()
{
    QFont font(m_font.family());
    font.setBold(m_font.bold());
    font.setItalic(m_font.italic());
    font.setPixelSize(m_font.pointSize() * 1.25);
    QFontMetrics fm(font);
    m_width = fm.width(m_text) + 2;
    m_height = fm.height();

    QString svg = "<svg width=\"" + QString::number(m_width) + "\" ";
    svg += "height=\"" + QString::number(m_height) + "\" >";
    svg += getTextTag("");
    svg += "</svg>";

    return svg;
}

Text::Text(QString text, AnimationScene *scene)
    : AnimationItem(scene)
{
   m_font = QFont("Arial");
   m_font.setPointSize(14);
   m_font.setStyleName("Standard");
   m_text = text;
   m_textcolor = QColor(Qt::black);

   m_data = getSvg().toUtf8();
   m_renderer = new QSvgRenderer();
   m_renderer->load(m_data);
   m_svg = new QGraphicsSvgItem(this);
   m_svg->setSharedRenderer(m_renderer);

   setRect(0, 0, m_svg->boundingRect().width(), m_svg->boundingRect().height());
}

int Text::type() const
{
    return Text::Type;
}

QDomElement Text::getXml(QDomDocument doc)
{
    QDomElement ele = doc.createElement("Text");
    writeAttributes(ele);
    ele.setAttribute("xscale", QVariant(xscale()).toString());
    ele.setAttribute("yscale", QVariant(yscale()).toString());
    ele.setAttribute("text", text());
    ele.setAttribute("textcolor", textColor().name());
    ele.setAttribute("opacity", opacity());
    ele.setAttribute("font-family", font().family());
    ele.setAttribute("font-size", font().pointSize());
    ele.setAttribute("font-style", font().styleName());
    return ele;
}

void Text::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}

void Text::setFont(QFont font)
{
    m_font = font;
    m_data = getSvg().toUtf8();
    m_renderer->load(m_data);
    m_svg->setSharedRenderer(m_renderer);
    setScale(1, 1);
    setHandlePositions();
}

QString Text::text()
{
    return m_text;
}

void Text::setText(QString text)
{
    m_text = text;
    m_data = getSvg().toUtf8();
    m_renderer->load(m_data);
    m_svg->setSharedRenderer(m_renderer);
    setHandlePositions();
}

QColor Text::textColor()
{
    return m_textcolor;
}

void Text::setTextColor(QColor color)
{
   m_textcolor = color;
   m_data = getSvg().toUtf8();
   m_renderer->load(m_data);
   m_svg->setSharedRenderer(m_renderer);
   emit textcolorChanged(color);
}

void Text::setScale(qreal x, qreal y)
{
    m_xscale = x;
    m_yscale = y;
    QTransform trans;
    trans.scale(m_xscale, m_yscale);
    m_svg->setTransform(trans);
    setRect(0, 0, m_svg->boundingRect().width() * x, m_svg->boundingRect().height() * y);
}

void Text::scaleObjects()
{
    m_xscale = rect().width() / m_svg->boundingRect().width();
    m_yscale = rect().height() / m_svg->boundingRect().height();
    QTransform trans;
    trans.scale(m_xscale, m_yscale);
    m_svg->setTransform(trans);
}
