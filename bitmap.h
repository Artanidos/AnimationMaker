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

#ifndef BITMAP_H
#define BITMAP_H

#include <QGraphicsItem>
#include "resizeableitem.h"

class Bitmap : public ResizeableItem
{
    Q_OBJECT
public:
    Bitmap();
    Bitmap(QString filename, AnimationScene *scene);
    Bitmap(QImage img, qreal width, qreal height, AnimationScene *scene);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);

    QImage getImage();
    void setImage(QImage image);

    enum { Type = UserType + 4 };
    int type() const Q_DECL_OVERRIDE;

private:
    QImage m_image;
};

#endif // BITMAP_H
