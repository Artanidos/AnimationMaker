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

#include <QGraphicsItem>
#include <QGraphicsSvgItem>
#include "resizeableitem.h"

class AnimationScene;
class Vectorgraphic : public ResizeableItem
{
public:
    Vectorgraphic(QString filename, AnimationScene *scene);
    Vectorgraphic(QByteArray arr, AnimationScene *scene);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    void scaleObjects();
    void setScale(qreal x, qreal y);
    QByteArray getData();

    enum { Type = UserType + 5 };
    int type() const Q_DECL_OVERRIDE;

private:
    QGraphicsSvgItem *m_svg;
    QSvgRenderer *m_renderer;
    QByteArray m_data;
};

#endif // VECTORGRAPHIC_H
