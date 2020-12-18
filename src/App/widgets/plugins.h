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

#ifndef PLUGINS_H
#define PLUGINS_H

#include <QList>
#include <QMap>
#include <QString>

#include "interfaces.h"

class  Plugins
{
public:
    static ItemInterface *getItemPlugin(QString name);
    static ExportInterface *getExportPlugin(QString name);
    static bool hasItemPlugin(QString name);
    static QList<QString> itemPluginNames();
    static QList<QString> exportPluginNames();
    static void insert(QString name, ItemInterface* plugin);
    static void insert(QString name, ExportInterface* plugin);

private:
    static QMap<QString, ItemInterface*> itemPlugins;
    static QMap<QString, ExportInterface*> exportPlugins;
};

#endif // PLUGINS_H
