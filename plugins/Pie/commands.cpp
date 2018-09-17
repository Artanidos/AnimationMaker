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
#include "animationscene.h"

ChangeStartAngleCommand::ChangeStartAngleCommand(int newValue, int oldValue, AnimationScene *scene, Pie *pie, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_oldValue = oldValue;
    m_newValue = newValue;
    m_pie = pie;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe  = nullptr;
    setText("Change Pie Start Angle");
}

void ChangeStartAngleCommand::undo()
{
    m_pie->setStartAngle(m_oldValue);
    m_pie->adjustKeyframes("startAngle", QVariant(m_oldValue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
    emit m_pie->startAngleChanged(m_oldValue);
}

void ChangeStartAngleCommand::redo()
{
    m_pie->setStartAngle(m_newValue);
    m_pie->adjustKeyframes("startAngle", QVariant(m_newValue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
    emit m_pie->startAngleChanged(m_newValue);
}

ChangeSpanAngleCommand::ChangeSpanAngleCommand(int newValue, int oldValue, AnimationScene *scene, Pie *pie, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_oldValue = oldValue;
    m_newValue = newValue;
    m_pie = pie;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe  = nullptr;
    setText("Change Pie Span Angle");
}

void ChangeSpanAngleCommand::undo()
{
    m_pie->setSpanAngle(m_oldValue);
    m_pie->adjustKeyframes("spanAngle", QVariant(m_oldValue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
    emit m_pie->spanAngleChanged(m_oldValue);
}

void ChangeSpanAngleCommand::redo()
{
    m_pie->setSpanAngle(m_newValue);
    m_pie->adjustKeyframes("spanAngle", QVariant(m_newValue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
    emit m_pie->spanAngleChanged(m_newValue);
}
