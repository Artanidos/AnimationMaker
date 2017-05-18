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
#include "timeline.h"
#include "text.h"

class AddItemCommand : public QUndoCommand
{
public:
    AddItemCommand(qreal x, qreal y, AnimationScene::EditMode mode, QString fileName, AnimationScene *scene, QUndoCommand *parent = 0);
    ~AddItemCommand();
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
    ~DeleteItemCommand();
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


class ChangeIdCommand : public QUndoCommand
{
public:
    ChangeIdCommand(QString id, QString oldid, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    QString m_id, m_oldid;
};

class ChangeColorCommand : public QUndoCommand
{
public:
    ChangeColorCommand(QColor color, QColor oldcolor, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    QColor m_color, m_oldcolor;
};

class ChangePenCommand : public QUndoCommand
{
public:
    ChangePenCommand(QColor color, QColor oldcolor, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    QColor m_color, m_oldcolor;
};

class ChangeTextcolorCommand : public QUndoCommand
{
public:
    ChangeTextcolorCommand(QColor color, QColor oldcolor, Text *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    Text *m_item;
    QColor m_color, m_oldcolor;
};

class ChangeSceneColorCommand : public QUndoCommand
{
public:
    ChangeSceneColorCommand(QColor color, QColor oldcolor, AnimationScene *scene, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationScene *m_scene;
    QColor m_color, m_oldcolor;
};

class ChangeOpacityCommand : public QUndoCommand
{
public:
    ChangeOpacityCommand(int opacity, int oldopacity, AnimationScene *scene, ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    int m_opacity, m_oldopacity;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
};

class AddKeyframeCommand : public QUndoCommand
{
public:
    AddKeyframeCommand(QString propertyname, KeyFrame *frame, ResizeableItem *item, Timeline *timeline, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    QString m_propertyName;
    KeyFrame *m_frame;
    Timeline *m_timeline;
};

class DeleteKeyframeCommand : public QUndoCommand
{
public:
    DeleteKeyframeCommand(QString propertyname, KeyFrame *frame, ResizeableItem *item, Timeline *timeline, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    QString m_propertyName;
    KeyFrame *m_frame;
    Timeline *m_timeline;
};

class AddTransitionCommand : public QUndoCommand
{
public:
    AddTransitionCommand(QString propertyname, KeyFrame *frame, ResizeableItem *item, Timeline *timeline, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    QString m_propertyName;
    KeyFrame *m_frame;
    Timeline *m_timeline;
};

class DeleteTransitionCommand : public QUndoCommand
{
public:
    DeleteTransitionCommand(QString propertyname, KeyFrame *frame, ResizeableItem *item, Timeline *timeline, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
    QString m_propertyName;
    KeyFrame *m_frame;
    Timeline *m_timeline;
};

class ChangeEasingCommand : public QUndoCommand
{
public:
    ChangeEasingCommand(int easing, int oldeasing, KeyFrame *frame, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    KeyFrame *m_frame;
    int m_easing, m_oldeasing;
};

class ChangeFontCommand : public QUndoCommand
{
public:
    ChangeFontCommand(QFont font, QFont oldfont, Text *text, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    Text *m_textitem;
    QFont m_font, m_oldfont;
};

class RaiseItemCommand : public QUndoCommand
{
public:
    RaiseItemCommand(ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
};

class LowerItemCommand : public QUndoCommand
{
public:
    LowerItemCommand(ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
};

class BringItemToFrontCommand : public QUndoCommand
{
public:
    BringItemToFrontCommand(ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
};

class SendItemToBackCommand : public QUndoCommand
{
public:
    SendItemToBackCommand(ResizeableItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    ResizeableItem *m_item;
};

#endif // COMMANDS_H
