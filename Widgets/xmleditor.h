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

#ifndef XMLEDITOR_H
#define XMLEDITOR_H

#include <QTextEdit>
#include "widgets_global.h"

class WIDGETSSHARED_EXPORT XmlEditor : public QTextEdit
{
    Q_OBJECT

public:
    XmlEditor();

    void focusOutEvent(QFocusEvent *e) {QTextEdit::focusOutEvent(e); emit editingFinished();}

signals:
    void editingFinished();
};

#endif // XMLEDITOR_H
