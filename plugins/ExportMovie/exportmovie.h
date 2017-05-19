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

#ifndef EXPORTMOVIE_H
#define EXPORTMOVIE_H

#include "interfaces.h"
#include <QObject>

class ExportMovie : public ExportMovieInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.artanidos.AnimationMaker.ExportMovieInterface" FILE "exports.json")
    Q_INTERFACES(ExportMovieInterface)

public:
    ExportMovie();
    QString displayName() const override;
    QString filter() const override;
    QString title() const override;
    void exportMovie(QString filename, QGraphicsView *view, int length, int fps, QStatusBar *bar) override;
signals:
    void setFrame(int);
};

#endif // EXPORTMOVIE_H
