/****************************************************************************
** Copyright (C) 2017 Olaf Japp
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

#ifndef PIEITEM_H
#define PIEITEM_H

#include <QtPlugin>
#include <QObject>
#include <QIcon>
#include <QImage>
#include <QPixmap>
#include <QCursor>
#include "interfaces.h"
#include "resizeableitem.h"

class PieItem : public ItemInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.github.Artanidos.AnimationMaker.ItemInterface")
    Q_INTERFACES(ItemInterface)

public:
    PieItem() {}
    QString tagName() override {return "Pie";}
    QString className() override {return "PieItem";}
    QString displayName() override {return "Pie Item";}
    QIcon icon() override {return QIcon(":/pie.png");}
    QCursor getCursor() override {return QCursor(QPixmap::fromImage(QImage(":/cursor.png")));}
    ResizeableItem *getInstance(AnimationScene *scene) override;
    ResizeableItem *getInstanceFromXml(AnimationScene *scene, QDomElement ele) override;
 };

class Pie : public ResizeableItem
{
    Q_OBJECT
public:
    Pie(AnimationScene *scene, QString typeName);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);

    QString typeName() {return m_typeName;}
    QDomElement getXml(QDomDocument);
    bool hasBrushAndPen() {return true;}
    QList<PropertyEditorInterface*> *getPropertyEditors() override;

    void setStartAngle(int angle);
    int startAngle() {return m_startAngle;}
    void setSpanAngle(int angle);
    int spanAngle() {return m_spanAngle;}

signals:
    void startAngleChanged(int angle);
    void spanAngleChanged(int angle);

private:
    int m_startAngle;
    int m_spanAngle;
    QString m_typeName;
};

#endif // PIEITEM_H
