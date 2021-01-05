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

#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QDomElement>
#include <QObject>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

#include "animationitem.h"
#include "animationscene.h"

class Ellipse : public AnimationItem
{
    Q_OBJECT
public:
    Ellipse(AnimationScene *scene);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    QString getHtml(QString id, QString assetsPath) Q_DECL_OVERRIDE;
    enum { Type = UserType + 2 };
    int type() const Q_DECL_OVERRIDE;
    QString typeName() {return "Ellipse";}
    QDomElement getXml(QDomDocument);
    bool hasBrushAndPen() {return true;}
};

#endif // ELLIPSE_H
