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

#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include <QWidget>
#include <QSpinBox>
#include "pieitem.h"
#include "propertyeditorinterface.h"

class PropertyEditor : public PropertyEditorInterface
{
    Q_OBJECT
public:
    explicit PropertyEditor();

    void setItem(AnimationItem *item) override;

public slots:
    void startAngleChanged();
    void spanAngleChanged();
    void startAngleChanged(int angle);
    void spanAngleChanged(int angle);
    void addStartKeyFrame();
    void addSpanKeyFrame();

private:
    QSpinBox *m_startAngle;
    QSpinBox *m_spanAngle;
    bool m_initializing;
    Pie *m_item;
};

#endif // PROPERTYEDITOR_H
