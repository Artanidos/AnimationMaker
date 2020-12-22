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

#include <QByteArray>
#include <QColor>
#include <QDomElement>
#include <QFont>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSvgItem>
#include <QObject>
#include <QPainter>
#include <QString>
#include <QStyleOptionGraphicsItem>
#include <QSvgRenderer>

#include "animationitem.h"
#include "animationscene.h"

class Text : public AnimationItem
{
    Q_OBJECT
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)

public:
    Text(QString text, AnimationScene *scene);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;
    void scaleObjects() Q_DECL_OVERRIDE;
    void setScale(qreal x, qreal y) Q_DECL_OVERRIDE;
    QString text();
    void setText(QString text);
    QString getHtml(QString id, QString assetsPath) Q_DECL_OVERRIDE;

    QColor textColor();
    void setTextColor(QColor textColor);

    inline QFont font() {return m_font;}
    void setFont(QFont font);

    enum { Type = UserType + 3 };
    int type() const Q_DECL_OVERRIDE;
    QString typeName() Q_DECL_OVERRIDE {return "Text";}
    QDomElement getXml(QDomDocument) Q_DECL_OVERRIDE;
    bool hasBrushAndPen() Q_DECL_OVERRIDE {return false;}

    QString getTextTag(QString id);
    
signals:
    void textcolorChanged(QColor);

private:
    QGraphicsSvgItem *m_svg;
    QSvgRenderer *m_renderer;
    QByteArray m_data;
    QFont m_font;
    QString m_text;
    QGraphicsSimpleTextItem *m_textitem;
    QColor m_textcolor;
    qreal m_width;
    qreal m_height;
    QString getSvg();
};

#endif // TEXT_H
