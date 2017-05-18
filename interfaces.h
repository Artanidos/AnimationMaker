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

#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QStatusBar>
#include <QGraphicsView>

class ExportInterface : public QObject
{
    Q_OBJECT
public:
    virtual QString displayName() const = 0;
    virtual QString filter() const = 0;
    virtual QString title() const = 0;
    virtual int exportFile(const char *filename, QGraphicsView *view, int length, QStatusBar *bar, int fps) = 0;
signals:
    void setPlayheadPos(int);
};

#define ExportInterface_iid "org.artanidos.AnimationMaker.ExportInterface"
Q_DECLARE_INTERFACE(ExportInterface, ExportInterface_iid)

#endif // INTERFACES_H
