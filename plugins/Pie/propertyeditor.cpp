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
#include "flatbutton.h"
#include "commands.h"
#include "animationscene.h"
#include "QLabel"

PropertyEditor::PropertyEditor()
{
    QVBoxLayout *layout = new QVBoxLayout;
    QGridLayout *grid = new QGridLayout;
    Expander *exp = new Expander("Angles");
    m_startAngle = new QSpinBox;
    m_spanAngle = new QSpinBox;
    m_startAngle->setMaximum(360);
    m_startAngle->setMinimum(-360);
    m_spanAngle->setMaximum(360);
    m_spanAngle->setMinimum(-360);
    FlatButton *addStartAngleKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addStartAngleKeyframe->setMaximumWidth(9);
    addStartAngleKeyframe->setToolTip("Add keyframe for start angle");
    FlatButton *addSpanAngleKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addSpanAngleKeyframe->setMaximumWidth(9);
    addSpanAngleKeyframe->setToolTip("Add keyframe for span angle");

    grid->addWidget(new QLabel("Start angle"), 0, 0);
    grid->addWidget(addStartAngleKeyframe, 0, 1);
    grid->addWidget(m_startAngle, 0, 2);
    grid->addWidget(new QLabel("Span angle"), 0, 3);
    grid->addWidget(addSpanAngleKeyframe, 0, 4);
    grid->addWidget(m_spanAngle, 0, 5);
    exp->addLayout(grid);
    layout->addWidget(exp);
    setLayout(layout);

    connect(m_startAngle, SIGNAL(valueChanged(int)), this, SLOT(startAngleChanged()));
    connect(m_spanAngle, SIGNAL(valueChanged(int)), this, SLOT(spanAngleChanged()));
}

void PropertyEditor::setItem(ResizeableItem *item)
{
    m_initializing = true;
    m_item = dynamic_cast<Pie*>(item);
    m_startAngle->setValue(m_item->startAngle());
    m_spanAngle->setValue(m_item->spanAngle());
    m_initializing = false;

    connect(item, SIGNAL(startAngleChanged(int)), this, SLOT(startAngleChanged(int)));
    connect(item, SIGNAL(spanAngleChanged(int)), this, SLOT(spanAngleChanged(int)));
}

void PropertyEditor::startAngleChanged()
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeStartAngleCommand(m_startAngle->value(), m_item->startAngle(), m_item);
        undoStack->push(cmd);
    }
}

void PropertyEditor::spanAngleChanged()
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeSpanAngleCommand(m_spanAngle->value(), m_item->spanAngle(), m_item);
        undoStack->push(cmd);
    }
}

void PropertyEditor::startAngleChanged(int angle)
{
    m_initializing = true;
    m_startAngle->setValue(angle);
    m_initializing = false;
}

void PropertyEditor::spanAngleChanged(int angle)
{
    m_initializing = true;
    m_spanAngle->setValue(angle);
    m_initializing = false;
}

