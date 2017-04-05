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
#include "text.h"

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
    MoveItemCommand(qreal x, qreal y, qreal oldx, qreal oldy, AnimationScene *scene, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    qreal m_x, m_y, m_oldx, m_oldy;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
};


class ResizeItemCommand : public QUndoCommand
{
public:
    ResizeItemCommand(qreal width, qreal height, qreal oldwidth, qreal oldheight, AnimationScene *scene, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    qreal m_width, m_height, m_oldwidth, m_oldheight;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
};


class ScaleItemCommand : public QUndoCommand
{
public:
    ScaleItemCommand(qreal x, qreal y, qreal width, qreal height, qreal oldx, qreal oldy, qreal oldwidth, qreal oldheight, AnimationScene *scene, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    qreal m_x, m_y, m_width, m_height, m_oldx, m_oldy, m_oldwidth, m_oldheight;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
};


class ChangeIdItemCommand : public QUndoCommand
{
public:
    ChangeIdItemCommand(QString id, QString oldid, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    QString m_id, m_oldid;
};

class ChangeColorItemCommand : public QUndoCommand
{
public:
    ChangeColorItemCommand(QColor color, QColor oldcolor, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    QColor m_color, m_oldcolor;
};

class ChangePenItemCommand : public QUndoCommand
{
public:
    ChangePenItemCommand(QColor color, QColor oldcolor, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    QColor m_color, m_oldcolor;
};

class ChangeTextcolorItemCommand : public QUndoCommand
{
public:
    ChangeTextcolorItemCommand(QColor color, QColor oldcolor, Text *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    Text *m_item;
    QColor m_color, m_oldcolor;
};
#endif // COMMANDS_H
