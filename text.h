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

#ifndef TEXT_H
#define TEXT_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QColor>
#include "resizeableitem.h"

class AnimationScene;
class Text : public ResizeableItem
{
    Q_OBJECT
public:
    Text(QString text, AnimationScene *scene);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    void scaleObjects();
    void setScale(qreal x, qreal y);
    QString text();
    void setText(QString text);

    QColor textcolor();
    void setTextcolor(QColor textcolor);

    inline QFont font() {return m_font;}
    void setFont(QFont font);

    enum { Type = UserType + 3 };
    int type() const Q_DECL_OVERRIDE;

signals:
    void textcolorChanged(QColor);

private:
    QFont m_font;
    QString m_text;
    QGraphicsSimpleTextItem *m_textitem;
    QColor m_textcolor;
    qreal m_width;
    qreal m_height;
};

#endif // TEXT_H
