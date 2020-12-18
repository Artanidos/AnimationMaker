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

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "pieitem.h"
#include "widgets_global.h"

class ChangeStartAngleCommand : public QUndoCommand
{
public:
    ChangeStartAngleCommand(int oldValue, int newValue, AnimationScene *scene, Pie *pie, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
   int m_oldValue;
   int m_newValue;
   Pie *m_pie;
   int m_time;
   bool m_autokeyframes;
   bool m_autotransition;
   KeyFrame *m_keyframe;
};

class ChangeSpanAngleCommand : public QUndoCommand
{
public:
    ChangeSpanAngleCommand(int newValue, int oldValue, AnimationScene *scene, Pie *pie, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
   int m_oldValue;
   int m_newValue;
   Pie *m_pie;
   int m_time;
   bool m_autokeyframes;
   bool m_autotransition;
   KeyFrame *m_keyframe;
};

#endif // COMMANDS_H
