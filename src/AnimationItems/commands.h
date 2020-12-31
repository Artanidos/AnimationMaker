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

#include <QByteArray>
#include <QColor>
#include <QFont>
#include <QString>
#include <QUndoCommand>

#include "animationitem.h"
#include "animationscene.h"
#include "bitmap.h"
#include "keyframe.h"
#include "text.h"
#include "timeline.h"
#include "vectorgraphic.h"

class AddItemCommand : public QUndoCommand
{
public:
    AddItemCommand(qreal x, qreal y, AnimationScene::EditMode mode, QString fileName, AnimationScene *scene, QUndoCommand *parent = 0);
    ~AddItemCommand();
    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    AnimationScene *m_scene;
};


class  DeleteItemCommand : public QUndoCommand
{
public:
    DeleteItemCommand(AnimationItem *item, AnimationScene *scene, QUndoCommand *parent = 0);
    ~DeleteItemCommand();
    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    AnimationScene *m_scene;
};


class MoveItemCommand : public QUndoCommand
{
public:
    MoveItemCommand(qreal x, qreal y, qreal oldx, qreal oldy, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    qreal m_x, m_y, m_oldx, m_oldy;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframeLeft;
    KeyFrame *m_keyframeTop;
};


class ResizeItemCommand : public QUndoCommand
{
public:
    ResizeItemCommand(qreal width, qreal height, qreal oldwidth, qreal oldheight, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    qreal m_width, m_height, m_oldwidth, m_oldheight;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframeWidth;
    KeyFrame *m_keyframeHeight;
};


class ScaleItemCommand : public QUndoCommand
{
public:
    ScaleItemCommand(qreal x, qreal y, qreal width, qreal height, qreal oldx, qreal oldy, qreal oldwidth, qreal oldheight, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    qreal m_x, m_y, m_width, m_height, m_oldx, m_oldy, m_oldwidth, m_oldheight;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframeLeft;
    KeyFrame *m_keyframeTop;
    KeyFrame *m_keyframeWidth;
    KeyFrame *m_keyframeHeight;
};


class ChangeIdCommand : public QUndoCommand
{
public:
    ChangeIdCommand(QString id, QString oldid, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    QString m_id, m_oldid;
};

class ChangeColorCommand : public QUndoCommand
{
public:
    ChangeColorCommand(QColor color, QColor oldcolor, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    QColor m_color, m_oldcolor;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangePenCommand : public QUndoCommand
{
public:
    ChangePenCommand(QColor color, QColor oldcolor, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    QColor m_color, m_oldcolor;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangeTextcolorCommand : public QUndoCommand
{
public:
    ChangeTextcolorCommand(QColor color, QColor oldcolor, AnimationScene *scene, Text *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    Text *m_item;
    QColor m_color, m_oldcolor;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangeOpacityCommand : public QUndoCommand
{
public:
    ChangeOpacityCommand(int opacity, int oldopacity, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    int m_opacity, m_oldopacity;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class AddKeyframeCommand : public QUndoCommand
{
public:
    AddKeyframeCommand(QString propertyname, KeyFrame *frame, AnimationItem *item, Timeline *timeline, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    QString m_propertyName;
    KeyFrame *m_frame;
    Timeline *m_timeline;
};

class DeleteKeyframeCommand : public QUndoCommand
{
public:
    DeleteKeyframeCommand(QString propertyname, KeyFrame *frame, AnimationItem *item, Timeline *timeline, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    QString m_propertyName;
    KeyFrame *m_frame;
    Timeline *m_timeline;
};

class AddTransitionCommand : public QUndoCommand
{
public:
    AddTransitionCommand(QString propertyname, KeyFrame *frame, AnimationItem *item, Timeline *timeline, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    QString m_propertyName;
    KeyFrame *m_frame;
    Timeline *m_timeline;
};

class DeleteTransitionCommand : public QUndoCommand
{
public:
    DeleteTransitionCommand(QString propertyname, KeyFrame *frame, AnimationItem *item, Timeline *timeline, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
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
    RaiseItemCommand(AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
};

class LowerItemCommand : public QUndoCommand
{
public:
    LowerItemCommand(AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
};

class BringItemToFrontCommand : public QUndoCommand
{
public:
    BringItemToFrontCommand(AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
};

class SendItemToBackCommand : public QUndoCommand
{
public:
    SendItemToBackCommand(AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
};

class ChangeAttributeCommand : public QUndoCommand
{
public:
    ChangeAttributeCommand(QString attributeName, QString newValue, QString oldValue, AnimationScene *scene, Vectorgraphic *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    Vectorgraphic *m_item;
    QString m_attributeName;
    QString m_newValue, m_oldValue;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangeSvgCommand : public QUndoCommand
{
public:
    ChangeSvgCommand(QByteArray newValue, QByteArray oldValue, Vectorgraphic *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    Vectorgraphic *m_item;
    QByteArray m_newValue, m_oldValue;
};

class RemoveAttributeCommand : public QUndoCommand
{
public:
    RemoveAttributeCommand(QString attributeName, QString value, Vectorgraphic *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    Vectorgraphic *m_item;
    QString m_attributeName;
    QString m_value;
};

class ExchangeBitmapCommand : public QUndoCommand
{
public:
    ExchangeBitmapCommand(Bitmap *item, QString fileName, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    Bitmap *m_item;
    QString m_fileName;
    QImage m_originalImage;
    QImage m_newImage;
};

class MoveKeyframeCommand : public QUndoCommand
{
public:
    MoveKeyframeCommand(KeyFrame *key, int oldTime, int newTime, Timeline *timeline, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    KeyFrame *m_key;
    int m_oldTime;
    int m_newTime;
    Timeline *m_timeline;
};

class MoveTransitionCommand : public QUndoCommand
{
public:
    MoveTransitionCommand(KeyFrame *key, int oldTime, int newTime, Timeline *timeline, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    KeyFrame *m_key;
    int m_oldTime;
    int m_newTime;
    Timeline *m_timeline;
};

class ResizeTransitionCommand : public QUndoCommand
{
public:
    ResizeTransitionCommand(KeyFrame *key, int oldStartTime, int newStartTime, int oldEndTime, int newEndTime, Timeline *timeline, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    KeyFrame *m_key;
    int m_oldStartTime;
    int m_newStartTime;
    int m_oldEndTime;
    int m_newEndTime;
    Timeline *m_timeline;
};

class ChangeScaleXCommand : public QUndoCommand
{
public:
    ChangeScaleXCommand(double value, double oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    double m_value, m_oldvalue;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangeScaleYCommand : public QUndoCommand
{
public:
    ChangeScaleYCommand(double value, double oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    double m_value, m_oldvalue;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangeShearXCommand : public QUndoCommand
{
public:
    ChangeShearXCommand(double value, double oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    double m_value, m_oldvalue;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangeShearYCommand : public QUndoCommand
{
public:
    ChangeShearYCommand(double value, double oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    double m_value, m_oldvalue;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangeRotationXCommand : public QUndoCommand
{
public:
    ChangeRotationXCommand(int value, int oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    int m_value, m_oldvalue;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangeRotationYCommand : public QUndoCommand
{
public:
    ChangeRotationYCommand(int value, int oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    int m_value, m_oldvalue;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangeRotationZCommand : public QUndoCommand
{
public:
    ChangeRotationZCommand(int value, int oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    int m_value, m_oldvalue;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangeOriginXCommand : public QUndoCommand
{
public:
    ChangeOriginXCommand(int value, int oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    int m_value, m_oldvalue;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

class ChangeOriginYCommand : public QUndoCommand
{
public:
    ChangeOriginYCommand(int value, int oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    int m_value, m_oldvalue;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

#endif // COMMANDS_H
