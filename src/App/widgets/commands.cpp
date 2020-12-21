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

#include "commands.h"

#include <QBrush>
#include <QDebug>
#include <QException>
#include <QGraphicsItem>
#include <QPen>
#include <QVariant>

#include "ellipse.h"
#include "interfaces.h"
#include "plugins.h"

AddItemCommand::AddItemCommand(qreal x, qreal y, AnimationScene::EditMode mode, QString fileName, AnimationScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_scene = scene;
    switch(mode)
    {
        case AnimationScene::EditMode::ModeSelect:
            // ignore
            break;
        case AnimationScene::EditMode::ModeRectangle:
        {
            m_item = new Rectangle(m_scene);
            m_item->setId("Rectangle");
            m_item->setPen(QPen(Qt::black));
            m_item->setBrush(QBrush(Qt::blue));
            m_item->setFlag(QGraphicsItem::ItemIsMovable, true);
            m_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            m_item->setPos(x, y);
            m_item->setWidth(50);
            m_item->setHeight(50);
            setText(QObject::tr("Add Rectangle"));
            break;
        }
        case AnimationScene::EditMode::ModeEllipse:
        {
            m_item = new Ellipse(m_scene);
            m_item->setId("Ellipse");
            m_item->setPen(QPen(Qt::black));
            m_item->setBrush(QBrush(Qt::blue));
            m_item->setFlag(QGraphicsItem::ItemIsMovable, true);
            m_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            m_item->setPos(x, y);
            m_item->setWidth(50);
            m_item->setHeight(50);
            setText(QObject::tr("Add Ellipse"));
            break;
        }
        case AnimationScene::EditMode::ModeText:
        {
            m_item = new Text("Lorem ipsum dolor", m_scene);
            m_item->setId("Text");
            m_item->setFlag(QGraphicsItem::ItemIsMovable, true);
            m_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            m_item->setPos(x, y);
            setText(QObject::tr("Add Text"));
            break;
        }
        case AnimationScene::EditMode::ModeBitmap:
        {
            m_item = new Bitmap(fileName, m_scene);
            m_item->setId("Bitmap");
            m_item->setFlag(QGraphicsItem::ItemIsMovable, true);
            m_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            m_item->setPos(x, y);
            setText(QObject::tr("Add Bitmap"));
            break;
        }
        case AnimationScene::EditMode::ModeSvg:
        {
            m_item = new Vectorgraphic(fileName, m_scene);
            m_item->setId("Vectorgraphic");
            m_item->setFlag(QGraphicsItem::ItemIsMovable, true);
            m_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            m_item->setPos(x, y);
            setText(QObject::tr("Add Vectorgraphic"));
            break;
        }
        case AnimationScene::EditMode::ModePlugin:
        {
            ItemInterface *item = Plugins::getItemPlugin(m_scene->actPluginName());
            m_item = item->getInstance(m_scene);
            m_item->setId(item->displayName());
            m_item->setFlag(QGraphicsItem::ItemIsMovable, true);
            m_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            m_item->setPos(x, y);
            m_item->setPlayheadPosition(m_scene->playheadPosition());
            setText("Add " + item->displayName());
            break;
        }
    }
}

AddItemCommand::~AddItemCommand()
{
    //if(m_item->isDeleted())
    //    delete m_item;
}

void AddItemCommand::undo()
{
    m_scene->clearSelection();
    m_scene->removeItem(m_item);
    m_item->setDeleted(true);
    emit m_scene->itemRemoved(m_item);
}

void AddItemCommand::redo()
{
    m_scene->clearSelection();
    m_scene->addItem(m_item);
    emit m_scene->itemAdded(m_item);
}


DeleteItemCommand::DeleteItemCommand(AnimationItem *item, AnimationScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_scene = scene;
    m_item = item;
    setText("Delete " + item->typeName());
}

DeleteItemCommand::~DeleteItemCommand()
{
    if(m_item->isDeleted())
    {
        try
        {
            delete m_item;
        }
        catch(QException)
        {
            qDebug() << "Exception catch on delete item";
        }
    }
}

void DeleteItemCommand::undo()
{
    m_scene->addItem(m_item);
    emit m_scene->itemAdded(m_item);
}

void DeleteItemCommand::redo()
{
    m_item->setSelected(false);
    m_scene->removeItem(m_item);
    m_item->setDeleted(true);
    emit m_scene->itemRemoved(m_item);
}

MoveItemCommand::MoveItemCommand(qreal x, qreal y, qreal oldx, qreal oldy, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_x = x;
    m_y = y;
    m_oldx = oldx;
    m_oldy = oldy;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_item = item;
    m_keyframeLeft = nullptr;
    m_keyframeTop = nullptr;
    setText("Move " + item->typeName());
}

void MoveItemCommand::undo()
{
    m_item->setPos(m_oldx, m_oldy);
    m_item->adjustKeyframes("left", QVariant(m_oldx), m_time, m_autokeyframes, m_autotransition, &m_keyframeLeft, true);
    m_item->adjustKeyframes("top", QVariant(m_oldy), m_time, m_autokeyframes, m_autotransition, &m_keyframeTop, true);
}

void MoveItemCommand::redo()
{
    m_item->setPos(m_x, m_y);
    m_item->adjustKeyframes("left", QVariant(m_x), m_time, m_autokeyframes, m_autotransition, &m_keyframeLeft, false);
    m_item->adjustKeyframes("top", QVariant(m_y), m_time, m_autokeyframes, m_autotransition, &m_keyframeTop, false);
}


ResizeItemCommand::ResizeItemCommand(qreal width, qreal height, qreal oldwidth, qreal oldheight, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_width = width;
    m_height = height;
    m_oldwidth = oldwidth;
    m_oldheight = oldheight;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_item = item;
    m_keyframeWidth = nullptr;
    m_keyframeHeight = nullptr;
    setText("Resize " + item->typeName());
}

void ResizeItemCommand::undo()
{
    m_item->setWidth(m_oldwidth);
    m_item->setHeight(m_oldheight);
    m_item->adjustKeyframes("width", QVariant(m_oldwidth), m_time, m_autokeyframes, m_autotransition, &m_keyframeWidth, true);
    m_item->adjustKeyframes("height", QVariant(m_oldheight), m_time, m_autokeyframes, m_autotransition, &m_keyframeHeight, true);
}

void ResizeItemCommand::redo()
{
    m_item->setWidth(m_width);
    m_item->setHeight(m_height);

    m_item->adjustKeyframes("width", QVariant(m_width), m_time, m_autokeyframes, m_autotransition, &m_keyframeWidth, false);
    m_item->adjustKeyframes("height", QVariant(m_height), m_time, m_autokeyframes, m_autotransition, &m_keyframeHeight, false);
}

ScaleItemCommand::ScaleItemCommand(qreal x, qreal y, qreal width, qreal height, qreal oldx, qreal oldy, qreal oldwidth, qreal oldheight, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_oldx = oldx;
    m_oldy = oldy;
    m_oldwidth = oldwidth;
    m_oldheight = oldheight;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_item = item;
    m_keyframeLeft = nullptr;
    m_keyframeTop = nullptr;
    m_keyframeWidth = nullptr;
    m_keyframeHeight = nullptr;
    setText("Scale " + item->typeName());
}

void ScaleItemCommand::undo()
{
    m_item->setPos(m_oldx, m_oldy);
    m_item->setWidth(m_oldwidth);
    m_item->setHeight(m_oldheight);
    m_item->adjustKeyframes("left", QVariant(m_oldx), m_time, m_autokeyframes, m_autotransition, &m_keyframeLeft, true);
    m_item->adjustKeyframes("top", QVariant(m_oldy), m_time, m_autokeyframes, m_autotransition, &m_keyframeTop, true);
    m_item->adjustKeyframes("width", QVariant(m_oldwidth), m_time, m_autokeyframes, m_autotransition, &m_keyframeWidth, true);
    m_item->adjustKeyframes("height", QVariant(m_oldheight), m_time, m_autokeyframes, m_autotransition, &m_keyframeHeight, true);
    m_item->scaleObjects();
    m_item->posChanged(m_oldx, m_oldy);
}

void ScaleItemCommand::redo()
{
    m_item->setPos(m_x, m_y);
    m_item->setWidth(m_width);
    m_item->setHeight(m_height);
    m_item->adjustKeyframes("left", QVariant(m_x), m_time, m_autokeyframes, m_autotransition, &m_keyframeLeft,false);
    m_item->adjustKeyframes("top", QVariant(m_y), m_time, m_autokeyframes, m_autotransition, &m_keyframeTop, false);
    m_item->adjustKeyframes("width", QVariant(m_width), m_time, m_autokeyframes, m_autotransition, &m_keyframeWidth, false);
    m_item->adjustKeyframes("height", QVariant(m_height), m_time, m_autokeyframes, m_autotransition, &m_keyframeHeight, false);
    m_item->scaleObjects();
    m_item->posChanged(m_x, m_y);
}

ChangeIdCommand::ChangeIdCommand(QString id, QString oldid, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_id = id;
    m_oldid = oldid;
    m_item = item;
    setText("Change " + item->typeName() + " Id");
}

void ChangeIdCommand::undo()
{
    m_item->setId(m_oldid);
}

void ChangeIdCommand::redo()
{
    m_item->setId(m_id);
}

ChangeColorCommand::ChangeColorCommand(QColor color, QColor oldcolor, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_color = color;
    m_oldcolor = oldcolor;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " Color");
}

void ChangeColorCommand::undo()
{
    m_item->setBrush(QBrush(m_oldcolor));
    m_item->adjustKeyframes("brushColor", QVariant(m_oldcolor), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangeColorCommand::redo()
{
    m_item->setBrush(QBrush(m_color));
    m_item->adjustKeyframes("brushColor", QVariant(m_color), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}

ChangePenCommand::ChangePenCommand(QColor color, QColor oldcolor, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_color = color;
    m_oldcolor = oldcolor;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " Pen");
}

void ChangePenCommand::undo()
{
    m_item->setPen(QPen(m_oldcolor));
    m_item->adjustKeyframes("penColor", QVariant(m_oldcolor), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangePenCommand::redo()
{
    m_item->setPen(QPen(m_color));
    m_item->adjustKeyframes("penColor", QVariant(m_color), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}

ChangeTextcolorCommand::ChangeTextcolorCommand(QColor color, QColor oldcolor, AnimationScene *scene, Text *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_color = color;
    m_oldcolor = oldcolor;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " Textcolor");
}

void ChangeTextcolorCommand::undo()
{
    m_item->setTextColor(m_oldcolor);
    m_item->adjustKeyframes("textColor", QVariant(m_oldcolor), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangeTextcolorCommand::redo()
{
    m_item->setTextColor(m_color);
    m_item->adjustKeyframes("textColor", QVariant(m_color), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}

ChangeOpacityCommand::ChangeOpacityCommand(int opacity, int oldopacity, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_opacity = opacity;
    m_oldopacity = oldopacity;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_item = item;
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " Opacity");
}

void ChangeOpacityCommand::undo()
{
    m_item->setOpacity(m_oldopacity);
    m_item->adjustKeyframes("opacity", QVariant(m_oldopacity), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangeOpacityCommand::redo()
{
    m_item->setOpacity(m_opacity);
    m_item->adjustKeyframes("opacity", QVariant(m_opacity), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}

AddKeyframeCommand::AddKeyframeCommand(QString propertyName, KeyFrame *frame, AnimationItem *item, Timeline *timeline, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_propertyName = propertyName;
    m_frame = frame;
    m_item = item;
    m_timeline = timeline;
    setText("Add " + item->typeName() + " Keyframe");
}

void AddKeyframeCommand::undo()
{
    m_timeline->deleteKeyFrame(m_item, m_propertyName, m_frame);
}

void AddKeyframeCommand::redo()
{
    m_timeline->addKeyFrame(m_item, m_propertyName, m_frame);
}

DeleteKeyframeCommand::DeleteKeyframeCommand(QString propertyName, KeyFrame *frame, AnimationItem *item, Timeline *timeline, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_propertyName = propertyName;
    m_frame = frame;
    m_item = item;
    m_timeline = timeline;
    setText("Delete " + item->typeName() + " Keyframe");
}

void DeleteKeyframeCommand::undo()
{
    m_timeline->addKeyFrame(m_item, m_propertyName, m_frame);
}

void DeleteKeyframeCommand::redo()
{
    m_timeline->deleteKeyFrame(m_item, m_propertyName, m_frame);
}

AddTransitionCommand::AddTransitionCommand(QString propertyName, KeyFrame *frame, AnimationItem *item, Timeline *timeline, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_propertyName = propertyName;
    m_frame = frame;
    m_item = item;
    m_timeline = timeline;
    setText("Add " + item->typeName() + " Transition");
}

void AddTransitionCommand::undo()
{
    m_timeline->deleteTransition(m_item, m_propertyName, m_frame);
}

void AddTransitionCommand::redo()
{
    m_timeline->addTransition(m_item, m_propertyName, m_frame);
}

DeleteTransitionCommand::DeleteTransitionCommand(QString propertyName, KeyFrame *frame, AnimationItem *item, Timeline *timeline, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_propertyName = propertyName;
    m_frame = frame;
    m_item = item;
    m_timeline = timeline;
    setText("Delete " + item->typeName() + " Transition");
}

void DeleteTransitionCommand::undo()
{
    m_timeline->addTransition(m_item, m_propertyName, m_frame);
}

void DeleteTransitionCommand::redo()
{
    m_timeline->deleteTransition(m_item, m_propertyName, m_frame);
}

ChangeEasingCommand::ChangeEasingCommand(int easing, int oldeasing, KeyFrame *frame, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_easing = easing;
    m_oldeasing = oldeasing;
    m_frame = frame;
    setText("Change  Easing");
}

void ChangeEasingCommand::undo()
{
    m_frame->setEasing(m_oldeasing);
}

void ChangeEasingCommand::redo()
{
    m_frame->setEasing(m_easing);
}

ChangeFontCommand::ChangeFontCommand(QFont font, QFont oldfont, Text *text, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_font = font;
    m_oldfont = oldfont;
    m_textitem = text;
    setText("Change  Font");
}

void ChangeFontCommand::undo()
{
    m_textitem->setFont(m_oldfont);
}

void ChangeFontCommand::redo()
{
    m_textitem->setFont(m_font);
}

RaiseItemCommand::RaiseItemCommand(AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_item = item;
    setText("Raise " + item->typeName());
}

void RaiseItemCommand::undo()
{
    m_item->lower();
}

void RaiseItemCommand::redo()
{
    m_item->raise();
}

LowerItemCommand::LowerItemCommand(AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_item = item;
    setText("Lower " + item->typeName());
}

void LowerItemCommand::undo()
{
    m_item->raise();
}

void LowerItemCommand::redo()
{
    m_item->lower();
}

BringItemToFrontCommand::BringItemToFrontCommand(AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_item = item;
    setText("Bring " + item->typeName() + " to front");
}

void BringItemToFrontCommand::undo()
{
    m_item->sendToBack();
}

void BringItemToFrontCommand::redo()
{
    m_item->bringToFront();
}

SendItemToBackCommand::SendItemToBackCommand(AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_item = item;
    setText("Send " + item->typeName() + " to back");
}

void SendItemToBackCommand::undo()
{
    m_item->bringToFront();
}

void SendItemToBackCommand::redo()
{
    m_item->sendToBack();
}

ChangeAttributeCommand::ChangeAttributeCommand(QString attributeName, QString newValue, QString oldValue, AnimationScene *scene, Vectorgraphic *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_attributeName = attributeName;
    m_newValue = newValue;
    m_oldValue = oldValue;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_item = item;
    m_keyframe = nullptr;
    setText("Change " + attributeName);
}

void ChangeAttributeCommand::undo()
{
    m_item->setAttributeValue(m_attributeName, m_oldValue);
    m_item->adjustKeyframes(m_attributeName, QVariant(m_oldValue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangeAttributeCommand::redo()
{
     m_item->setAttributeValue(m_attributeName, m_newValue);
     m_item->adjustKeyframes(m_attributeName, QVariant(m_newValue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}


ChangeSvgCommand::ChangeSvgCommand(QByteArray newValue, QByteArray oldValue, Vectorgraphic *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_newValue = newValue;
    m_oldValue = oldValue;
    m_item = item;
    setText("Change SVG");
}

void ChangeSvgCommand::undo()
{
    m_item->setData(m_oldValue);
    emit m_item->dataChanged();
}

void ChangeSvgCommand::redo()
{
    m_item->setData(m_newValue);
    emit m_item->dataChanged();
}

RemoveAttributeCommand::RemoveAttributeCommand(QString attributeName, QString value, Vectorgraphic *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_item = item;
    m_attributeName = attributeName;
    m_value = value;
    setText("Remove Attribute " + attributeName);
}

void RemoveAttributeCommand::undo()
{
    m_item->setAttributeValue(m_attributeName, m_value);
    emit m_item->attributeAdded();
}

void RemoveAttributeCommand::redo()
{
    m_item->removeAttribute(m_attributeName);
}

ExchangeBitmapCommand::ExchangeBitmapCommand(Bitmap *item, QString fileName, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_item = item;
    m_fileName = fileName;
    m_originalImage = item->getImage();
    m_newImage.load(fileName);
    setText("Exchange Bitmap");
}

void ExchangeBitmapCommand::undo()
{
    m_item->setImage(m_originalImage);
    m_item->update();
}

void ExchangeBitmapCommand::redo()
{
    m_item->setImage(m_newImage);
    m_item->update();
}

MoveKeyframeCommand::MoveKeyframeCommand(KeyFrame *key, int oldTime, int newTime, Timeline *timeline, QUndoCommand *parent)
    : QUndoCommand (parent)
{
    m_key = key;
    m_oldTime = oldTime;
    m_newTime = newTime;
    m_timeline = timeline;
    setText("Move keyframe");
}

void MoveKeyframeCommand::undo()
{
    m_timeline->moveKeyframe(m_key, m_oldTime);
}

void MoveKeyframeCommand::redo()
{
    m_timeline->moveKeyframe(m_key, m_newTime);
}

MoveTransitionCommand::MoveTransitionCommand(KeyFrame *key, int oldTime, int newTime, Timeline *timeline, QUndoCommand *parent)
    : QUndoCommand (parent)
{
    m_key = key;
    m_oldTime = oldTime;
    m_newTime = newTime;
    m_timeline = timeline;
    setText("Move transition");
}

void MoveTransitionCommand::undo()
{
    m_timeline->moveTransition(m_key, m_oldTime);
}

void MoveTransitionCommand::redo()
{
    m_timeline->moveTransition(m_key, m_newTime);
}

ResizeTransitionCommand::ResizeTransitionCommand(KeyFrame *key, int oldStartTime, int newStartTime, int oldEndTime, int newEndTime, Timeline *timeline, QUndoCommand *parent)
    : QUndoCommand (parent)
{
    m_key = key;
    m_oldStartTime = oldStartTime;
    m_oldEndTime = oldEndTime;
    m_newStartTime = newStartTime;
    m_newEndTime = newEndTime;
    m_timeline = timeline;
    setText("Resize transition");
}

void ResizeTransitionCommand::undo()
{
    m_timeline->resizeTransition(m_key, m_oldStartTime, m_oldEndTime);
}

void ResizeTransitionCommand::redo()
{
    m_timeline->resizeTransition(m_key, m_newStartTime, m_newEndTime);
}

ChangeScaleXCommand::ChangeScaleXCommand(double value, double oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_value = value;
    m_oldvalue = oldvalue;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " scale X");
}

void ChangeScaleXCommand::undo()
{
    m_item->setScaleX(m_oldvalue);
    m_item->adjustKeyframes("scalex", QVariant(m_oldvalue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangeScaleXCommand::redo()
{
    m_item->setScaleX(m_value);
    m_item->adjustKeyframes("scalex", QVariant(m_value), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}

ChangeScaleYCommand::ChangeScaleYCommand(double value, double oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_value = value;
    m_oldvalue = oldvalue;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " scale y");
}

void ChangeScaleYCommand::undo()
{
    m_item->setScaleY(m_oldvalue);
    m_item->adjustKeyframes("scaley", QVariant(m_oldvalue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangeScaleYCommand::redo()
{
    m_item->setScaleY(m_value);
    m_item->adjustKeyframes("scaley", QVariant(m_value), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}

ChangeShearXCommand::ChangeShearXCommand(double value, double oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_value = value;
    m_oldvalue = oldvalue;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " shear X");
}

void ChangeShearXCommand::undo()
{
    m_item->setShearX(m_oldvalue);
    m_item->adjustKeyframes("shearx", QVariant(m_oldvalue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangeShearXCommand::redo()
{
    m_item->setShearX(m_value);
    m_item->adjustKeyframes("shearx", QVariant(m_value), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}

ChangeShearYCommand::ChangeShearYCommand(double value, double oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_value = value;
    m_oldvalue = oldvalue;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " shear y");
}

void ChangeShearYCommand::undo()
{
    m_item->setShearY(m_oldvalue);
    m_item->adjustKeyframes("sheary", QVariant(m_oldvalue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangeShearYCommand::redo()
{
    m_item->setShearY(m_value);
    m_item->adjustKeyframes("sheary", QVariant(m_value), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}

ChangeRotationXCommand::ChangeRotationXCommand(int value, int oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_value = value;
    m_oldvalue = oldvalue;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " rotation X");
}

void ChangeRotationXCommand::undo()
{
    m_item->setRotationX(m_oldvalue);
    m_item->adjustKeyframes("rotationx", QVariant(m_oldvalue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangeRotationXCommand::redo()
{
    m_item->setRotationX(m_value);
    m_item->adjustKeyframes("rotationx", QVariant(m_value), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}

ChangeRotationYCommand::ChangeRotationYCommand(int value, int oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_value = value;
    m_oldvalue = oldvalue;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " rotation Y");
}

void ChangeRotationYCommand::undo()
{
    m_item->setRotationY(m_oldvalue);
    m_item->adjustKeyframes("rotationy", QVariant(m_oldvalue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangeRotationYCommand::redo()
{
    m_item->setRotationY(m_value);
    m_item->adjustKeyframes("rotationy", QVariant(m_value), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}

ChangeRotationZCommand::ChangeRotationZCommand(int value, int oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_value = value;
    m_oldvalue = oldvalue;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " rotation Z");
}

void ChangeRotationZCommand::undo()
{
    m_item->setRotationZ(m_oldvalue);
    m_item->adjustKeyframes("rotationz", QVariant(m_oldvalue), m_time, m_autokeyframes, m_autotransition, &m_keyframe, true);
}

void ChangeRotationZCommand::redo()
{
    m_item->setRotationZ(m_value);
    m_item->adjustKeyframes("rotationz", QVariant(m_value), m_time, m_autokeyframes, m_autotransition, &m_keyframe, false);
}

ChangeOriginXCommand::ChangeOriginXCommand(int value, int oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_value = value;
    m_oldvalue = oldvalue;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " origin X");
}

void ChangeOriginXCommand::undo()
{
    m_item->setOriginX(m_oldvalue);
}

void ChangeOriginXCommand::redo()
{
    m_item->setOriginX(m_value);
}

ChangeOriginYCommand::ChangeOriginYCommand(int value, int oldvalue, AnimationScene *scene, AnimationItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_value = value;
    m_oldvalue = oldvalue;
    m_item = item;
    m_time = scene->playheadPosition();
    m_autokeyframes = scene->autokeyframes();
    m_autotransition = scene->autotransition();
    m_keyframe = nullptr;
    setText("Change " + item->typeName() + " origin Y");
}

void ChangeOriginYCommand::undo()
{
    m_item->setOriginY(m_oldvalue);
}

void ChangeOriginYCommand::redo()
{
    m_item->setOriginY(m_value);
}
