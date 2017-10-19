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

#include "propertyeditor.h"
#include "expander.h"
#include "commands.h"
#include "QLabel"

PropertyEditor::PropertyEditor()
{
    QVBoxLayout *layout = new QVBoxLayout;
    QGridLayout *grid = new QGridLayout;
    Expander *exp = new Expander("Sample");

    grid->addWidget(new QLabel("Sample"), 0, 0);
    exp->addLayout(grid);
    layout->addWidget(exp);
    setLayout(layout);

    //connect(m_startAngle, SIGNAL(valueChanged(int)), this, SLOT(startAngleChanged()));
}
