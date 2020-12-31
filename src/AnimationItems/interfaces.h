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

#ifndef INTERFACES_H
#define INTERFACES_H

#include <QCursor>
#include <QDomElement>
#include <QIcon>
#include <QStatusBar>
#include <QString>

#include "animationitem.h"
#include "animationscene.h"

class ItemInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~ItemInterface() {}
    virtual QString tagName() = 0;
    virtual QString className() = 0;
    virtual QString displayName() = 0;
    virtual QIcon icon() = 0;
    virtual QCursor getCursor() = 0;
    virtual AnimationItem *getInstance(AnimationScene *) = 0;
    virtual AnimationItem *getInstanceFromXml(AnimationScene *, QDomElement) = 0;
};

QT_BEGIN_NAMESPACE
#define ItemInterface_iid "com.github.Artanidos.AnimationMaker.ItemInterface"

Q_DECLARE_INTERFACE(ItemInterface, ItemInterface_iid)
QT_END_NAMESPACE

class ExportInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~ExportInterface() {}
    virtual QString className() = 0;
    virtual QString displayName() = 0;
    virtual void exportAnimation(AnimationScene *scene, QStatusBar *bar) = 0;
};

QT_BEGIN_NAMESPACE
#define ExportInterface_iid "com.github.Artanidos.AnimationMaker.ExportInterface"

Q_DECLARE_INTERFACE(ExportInterface, ExportInterface_iid)
QT_END_NAMESPACE

#endif // INTERFACES_H
