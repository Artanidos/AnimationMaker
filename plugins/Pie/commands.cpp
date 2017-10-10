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

#include "commands.h"


ChangeStartAngleCommand::ChangeStartAngleCommand(int newValue, int oldValue, Pie *pie, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_oldValue = oldValue;
    m_newValue = newValue;
    m_pie = pie;
    setText("Change Pie Start Angle");
}

void ChangeStartAngleCommand::undo()
{
    m_pie->setStartAngle(m_oldValue);
    emit m_pie->startAngleChanged(m_oldValue);
}

void ChangeStartAngleCommand::redo()
{
    m_pie->setStartAngle(m_newValue);
    emit m_pie->startAngleChanged(m_newValue);
}

ChangeSpanAngleCommand::ChangeSpanAngleCommand(int newValue, int oldValue, Pie *pie, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_oldValue = oldValue;
    m_newValue = newValue;
    m_pie = pie;
    setText("Change Pie Span Angle");
}

void ChangeSpanAngleCommand::undo()
{
    m_pie->setSpanAngle(m_oldValue);
    emit m_pie->spanAngleChanged(m_oldValue);
}

void ChangeSpanAngleCommand::redo()
{
    m_pie->setSpanAngle(m_newValue);
    emit m_pie->spanAngleChanged(m_newValue);
}
