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

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "animationscene.h"
#include "resizeableitem.h"

class AddItemCommand : public QUndoCommand
{
public:
    AddItemCommand(qreal x, qreal y, AnimationScene::EditMode mode, QString fileName, AnimationScene *scene, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    AnimationScene *m_scene;
};


class DeleteItemCommand : public QUndoCommand
{
public:
    DeleteItemCommand(ResizeableItem *item, AnimationScene *scene, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    AnimationScene *m_scene;
};


class MoveItemCommand : public QUndoCommand
{
public:
    MoveItemCommand(qreal x, qreal y, qreal oldx, qreal oldy, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    qreal m_x, m_y, m_oldx, m_oldy;
};


class ResizeItemCommand : public QUndoCommand
{
public:
    ResizeItemCommand(qreal width, qreal height, qreal oldwidth, qreal oldheight, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    qreal m_width, m_height, m_oldwidth, m_oldheight;
};




#endif // COMMANDS_H
