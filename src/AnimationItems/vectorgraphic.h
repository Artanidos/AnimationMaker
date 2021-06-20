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

#ifndef VECTORGRAPHIC_H
#define VECTORGRAPHIC_H

#include <QByteArray>
#include <QDomElement>
#include <QGraphicsSvgItem>
#include <QHash>
#include <QObject>
#include <QPainter>
#include <QString>
#include <QStringList>
#include <QStyleOptionGraphicsItem>
#include <QSvgRenderer>

#include "animationitem.h"
#include "animationscene.h"

class Vectorgraphic : public AnimationItem
{
    Q_OBJECT
public:
    Vectorgraphic();
    Vectorgraphic(QString filename, AnimationScene *scene);
    Vectorgraphic(QByteArray arr, AnimationScene *scene);
    ~Vectorgraphic();
    QString getHtml(QString id, QString assetsPath) Q_DECL_OVERRIDE;
    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    void scaleObjects();
    void setScale(qreal x, qreal y);
    QByteArray getData();
    void setData(QByteArray data);

    void setAttributeValue(QString attributeName, QString value);
    void removeAttribute(QString attributeName);
    void changeAttributeName(QString oldName, QString newName);
    QHash<QString, QString> attributes() {return m_attributes;}

    enum { Type = UserType + SVG_TYPE };
    int type() const Q_DECL_OVERRIDE;
    QString typeName() {return "Vectorgraphic";}
    QDomElement getXml(QDomDocument);
    bool hasBrushAndPen() {return false;}

    QStringList getPropertyList() Q_DECL_OVERRIDE;

signals:
    void attributeAdded();
    void dataChanged();

private:
    QGraphicsSvgItem *m_svg;
    QSvgRenderer *m_renderer;
    QByteArray m_data;
    QByteArray m_changedData;
    QHash<QString, QString> m_attributes;

    QDomElement elementById(QDomElement root, QString id);
    void reload();
};

#endif // VECTORGRAPHIC_H
