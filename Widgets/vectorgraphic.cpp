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

#include "vectorgraphic.h"
#include "animationscene.h"
#include <QtTest/QTest>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>
#include <QSvgRenderer>
#include <QDomComment>

Vectorgraphic::Vectorgraphic()
    : AnimationItem(NULL)
{
    m_renderer = new QSvgRenderer();
    m_svg = new QGraphicsSvgItem(this);
    m_svg->setSharedRenderer(m_renderer);
}

Vectorgraphic::Vectorgraphic(QString filename, AnimationScene *scene)
    : AnimationItem(scene)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    m_data = file.readAll();
    m_changedData = m_data;
    m_renderer = new QSvgRenderer();
    m_renderer->load(m_data);
    m_svg = new QGraphicsSvgItem(this);
    m_svg->setSharedRenderer(m_renderer);

    setRect(0, 0, m_svg->boundingRect().width(), m_svg->boundingRect().height());
}

Vectorgraphic::Vectorgraphic(QByteArray arr, AnimationScene *scene)
    : AnimationItem(scene)
{
    m_data = arr;
    m_changedData = m_data;
    m_renderer = new QSvgRenderer();
    m_renderer->load(m_data);
    m_svg = new QGraphicsSvgItem(this);
    m_svg->setSharedRenderer(m_renderer);

    setRect(0, 0, m_svg->boundingRect().width(), m_svg->boundingRect().height());
}

QString Vectorgraphic::getHtml(QString, QString)
{
    return QString(m_data);
}

QDomElement Vectorgraphic::getXml(QDomDocument doc)
{
    QDomElement ele = doc.createElement("Vectorgraphic");
    writeAttributes(ele);
    ele.setAttribute("xscale", QVariant(xscale()).toString());
    ele.setAttribute("yscale", QVariant(yscale()).toString());
    ele.appendChild(doc.createCDATASection(QString::fromLatin1(getData()).replace("]]>", "]]]]><![CDATA[>")));
    QDomElement atts = doc.createElement("Attributes");
    for(int i = 0; i < attributes().count(); i++)
    {
        QString key = attributes().keys().at(i);
        QDomElement attribute = doc.createElement("Attribute");
        attribute.setAttribute("attributeName", key);
        attribute.setAttribute("value", attributes().value(key));
        atts.appendChild(attribute);
    }
    ele.appendChild(atts);
    return ele;
}

QString Vectorgraphic::getInnerSvg()
{
    QString svg = QString(m_data);
    int start = svg.indexOf("<", svg.indexOf("<svg") + 1);
    int end = svg.lastIndexOf("</svg>");
    return "<g transform=\"scale(" + QString::number(xscale()) +"," + QString::number(yscale()) + ")\">" + svg.mid(start, end - start) + "</g>";
}

QStringList Vectorgraphic::getPropertyList()
{
    QStringList list;
    foreach(QString key, m_attributes.keys())
    {
        list << key;
    }
    return list;
}

void Vectorgraphic::setData(QByteArray data)
{
    m_data = data;
    m_changedData = data;
    reload();
}

void Vectorgraphic::reload()
{
    QDomDocument doc;
    doc.setContent(m_data);

    foreach(QString key, m_attributes.keys())
    {
        QString value = m_attributes.value(key);

        int dot = key.indexOf(".");
        QString id = key.mid(0, dot);
        QString attribute = key.mid(dot + 1);

        QDomElement ele = elementById(doc.documentElement(), id);
        if(!ele.isNull())
        {
            ele.setAttribute(attribute, value);
        }
    }

    m_renderer->load(doc.toString().toUtf8());
    m_svg->setSharedRenderer(m_renderer);
    setRect(0, 0, m_svg->boundingRect().width(), m_svg->boundingRect().height());
}

int Vectorgraphic::type() const
{
    return Vectorgraphic::Type;
}

void Vectorgraphic::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}

void Vectorgraphic::scaleObjects()
{
    m_xscale = rect().width() / m_svg->boundingRect().width();
    m_yscale = rect().height() / m_svg->boundingRect().height();

    QTransform trans;
    trans.scale(m_xscale, m_yscale);
    m_svg->setTransform(trans);
}

void Vectorgraphic::setScale(qreal x, qreal y)
{
    m_xscale = x;
    m_yscale = y;
    QTransform trans;
    trans.scale(m_xscale, m_yscale);
    m_svg->setTransform(trans);

    setRect(0, 0, m_svg->boundingRect().width() * x, m_svg->boundingRect().height() * y);
}


QByteArray Vectorgraphic::getData()
{
    return m_data;
}

void Vectorgraphic::setAttributeValue(QString attributeName, QString value)
{
    if(m_attributes.contains(attributeName))
        m_attributes[attributeName] = value;
    else
        m_attributes.insert(attributeName, value);

    reload();
}

void Vectorgraphic::removeAttribute(QString attributeName)
{
    m_attributes.remove(attributeName);
    reload();
}

void Vectorgraphic::changeAttributeName(QString oldName, QString newName)
{
    QString value = m_attributes[oldName];
    m_attributes.remove(oldName);
    m_attributes[newName] = value;
    reload();
}

QDomElement Vectorgraphic::elementById(QDomElement root, QString id)
{
    for(int i = 0; i < root.childNodes().count(); i++)
    {
        QDomNode node = root.childNodes().at(i);
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            if(element.attribute("id") == id)
                return element;
            if(element.hasChildNodes())
            {
                QDomElement el = elementById(element, id);
                if(!el.isNull())
                    return el;
            }
        }
    }
    return QDomElement();
}
