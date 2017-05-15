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

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include "resizeableitem.h"

class Rectangle : public ResizeableItem
{
    Q_OBJECT
public:
    Rectangle() {}
    Rectangle(qreal width, qreal height, AnimationScene *scene);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);

    enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE;
};

#endif // RECTANGLE_H
