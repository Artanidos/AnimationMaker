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

#ifndef HTMLEXPORT_H
#define HTMLEXPORT_H

#include <QtPlugin>
#include <QObject>
#include <QIcon>
#include <QImage>
#include <QPixmap>
#include <QCursor>
#include "interfaces.h"
#include "animationitem.h"

class HtmlExport : public ExportInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.github.Artanidos.AnimationMaker.ExportInterface")
    Q_INTERFACES(ExportInterface)

public:
    HtmlExport() {}
    QString className() override {return "HtmlExport";}
    QString displayName() override {return "Html Export";}
    void exportAnimation(AnimationScene *scene, QStatusBar *) override;
 };

#endif // HTMLEXPORT_H
