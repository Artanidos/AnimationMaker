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
#include "animationitem.h"
#include "vectorgraphic.h"
#include "bitmap.h"
#include "timeline.h"
#include "text.h"
#include "widgets_global.h"

class WIDGETSSHARED_EXPORT AddItemCommand : public QUndoCommand
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


class WIDGETSSHARED_EXPORT DeleteItemCommand : public QUndoCommand
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


class WIDGETSSHARED_EXPORT MoveItemCommand : public QUndoCommand
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


class WIDGETSSHARED_EXPORT ResizeItemCommand : public QUndoCommand
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


class WIDGETSSHARED_EXPORT ScaleItemCommand : public QUndoCommand
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


class WIDGETSSHARED_EXPORT ChangeIdCommand : public QUndoCommand
{
public:
    ChangeIdCommand(QString id, QString oldid, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    QString m_id, m_oldid;
};

class WIDGETSSHARED_EXPORT ChangeColorCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ChangePenCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ChangeTextcolorCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ChangeOpacityCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT AddKeyframeCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT DeleteKeyframeCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT AddTransitionCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT DeleteTransitionCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ChangeEasingCommand : public QUndoCommand
{
public:
    ChangeEasingCommand(int easing, int oldeasing, KeyFrame *frame, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    KeyFrame *m_frame;
    int m_easing, m_oldeasing;
};

class WIDGETSSHARED_EXPORT ChangeFontCommand : public QUndoCommand
{
public:
    ChangeFontCommand(QFont font, QFont oldfont, Text *text, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    Text *m_textitem;
    QFont m_font, m_oldfont;
};

class WIDGETSSHARED_EXPORT RaiseItemCommand : public QUndoCommand
{
public:
    RaiseItemCommand(AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
};

class WIDGETSSHARED_EXPORT LowerItemCommand : public QUndoCommand
{
public:
    LowerItemCommand(AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
};

class WIDGETSSHARED_EXPORT BringItemToFrontCommand : public QUndoCommand
{
public:
    BringItemToFrontCommand(AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
};

class WIDGETSSHARED_EXPORT SendItemToBackCommand : public QUndoCommand
{
public:
    SendItemToBackCommand(AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
};

class WIDGETSSHARED_EXPORT ChangeAttributeCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ChangeSvgCommand : public QUndoCommand
{
public:
    ChangeSvgCommand(QByteArray newValue, QByteArray oldValue, Vectorgraphic *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    Vectorgraphic *m_item;
    QByteArray m_newValue, m_oldValue;
};

class WIDGETSSHARED_EXPORT RemoveAttributeCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ExchangeBitmapCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT MoveKeyframeCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT MoveTransitionCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ResizeTransitionCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ChangeScaleXCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ChangeScaleYCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ChangeShearXCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ChangeShearYCommand : public QUndoCommand
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

class WIDGETSSHARED_EXPORT ChangeTransXCommand : public QUndoCommand
{
public:
    ChangeTransXCommand(double value, double oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

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

class WIDGETSSHARED_EXPORT ChangeTransYCommand : public QUndoCommand
{
public:
    ChangeTransYCommand(double value, double oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

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

class WIDGETSSHARED_EXPORT ChangeRotationCommand : public QUndoCommand
{
public:
    ChangeRotationCommand(QString value, QString oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    AnimationItem *m_item;
    QString m_value, m_oldvalue;
    int m_time;
    bool m_autokeyframes;
    bool m_autotransition;
    KeyFrame *m_keyframe;
};

#endif // COMMANDS_H
