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

#ifndef EXPORTXML_H
#define EXPORTXML_H

#include "interfaces.h"
#include <QObject>
#include <QDomDocument>

class ExportXml : public ExportMetaInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.artanidos.AnimationMaker.ExportMetaInterface" FILE "exports.json")
    Q_INTERFACES(ExportMetaInterface)
public:
    ExportXml();
    QString displayName() const override;
    QString filter() const override;
    QString title() const override;
    void exportMeta(QString filename, AnimationMaker::Animation *animation, bool exportAll, QStatusBar *bar) override;
private:
    void writeKeyframes(QDomDocument *doc, QDomElement *element, AnimationMaker::AnimationItem *item);
};

#endif // EXPORTXML_H
