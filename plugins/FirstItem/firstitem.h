/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of FlatSiteBuilder.
**
**  FlatSiteBuilder is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  FlatSiteBuilder is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with FlatSiteBuilder.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef FIRSTITEM_H
#define FIRSTITEM_H

#include <QtPlugin>
#include <QObject>
#include <QIcon>
#include <QImage>
#include <QPixmap>
#include <QCursor>
#include "interfaces.h"
#include "resizeableitem.h"

class FirstItem : public ItemInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.github.Artanidos.AnimationMaker.ItemInterface")
    Q_INTERFACES(ItemInterface)

public:
    FirstItem() {}
    QString tagName() override {return "First";}
    QString className() override {return "FirstItem";}
    QString displayName() override {return "First Item";}
    QIcon icon() override {return QIcon(":/sample.png");}
    QCursor getCursor() override {return QCursor(QPixmap::fromImage(QImage(":/cursor.png")));}
    ResizeableItem *getInstance(AnimationScene *scene) override;
    ResizeableItem *getInstanceFromXml(AnimationScene *scene, QDomElement ele) override;
 };

class First : public ResizeableItem
{
    Q_OBJECT
public:
    First(AnimationScene *scene);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);

    //enum { Type = UserType + 2 };
    //int type() const Q_DECL_OVERRIDE;
    QString typeName() {return "First";}
    QDomElement getXml(QDomDocument);
};

#endif // FIRSTITEM_H
