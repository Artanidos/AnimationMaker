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
#include "rectangle.h"
#include "ellipse.h"
#include "text.h"
#include "bitmap.h"
#include "vectorgraphic.h"

AddItemCommand::AddItemCommand(qreal x, qreal y, AnimationScene::EditMode mode, QString fileName, AnimationScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_scene = scene;
    switch(mode)
    {
        case AnimationScene::EditMode::ModeRectangle:
        {
            m_item = new Rectangle(50, 50, m_scene);
            m_item->setId("Rectangle");
            m_item->setPen(QPen(Qt::black));
            m_item->setBrush(QBrush(Qt::blue));
            m_item->setFlag(QGraphicsItem::ItemIsMovable, true);
            m_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            m_item->setPos(x, y);
            setText(QObject::tr("Add Rectangle"));
            break;
        }
        case AnimationScene::EditMode::ModeEllipse:
        {
            m_item = new Ellipse(50, 50, m_scene);
            m_item->setId("Ellipse");
            m_item->setPen(QPen(Qt::black));
            m_item->setBrush(QBrush(Qt::blue));
            m_item->setFlag(QGraphicsItem::ItemIsMovable, true);
            m_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            m_item->setPos(x, y);
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
    }
}

void AddItemCommand::undo()
{
    m_scene->clearSelection();
    m_scene->removeItem(m_item);
    emit m_scene->itemRemoved(m_item);
}

void AddItemCommand::redo()
{
    m_scene->clearSelection();
    m_scene->addItem(m_item);
    emit m_scene->itemAdded(m_item);
}


DeleteItemCommand::DeleteItemCommand(ResizeableItem *item, AnimationScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_scene = scene;
    m_item = item;
    setText("Delete " + getItemTypeName(item));
}

void DeleteItemCommand::undo()
{
    m_scene->clearSelection();
    m_scene->addItem(m_item);
    emit m_scene->itemAdded(m_item);
}

void DeleteItemCommand::redo()
{
    m_scene->clearSelection();
    m_scene->removeItem(m_item);
    emit m_scene->itemRemoved(m_item);
}

MoveItemCommand::MoveItemCommand(qreal x, qreal y, qreal oldx, qreal oldy, AnimationScene *scene, ResizeableItem *item, QUndoCommand *parent)
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
    setText("Move " + getItemTypeName(item));
}

void MoveItemCommand::undo()
{
    m_item->setPos(m_oldx, m_oldy);
    m_item->adjustKeyframes("left", QVariant(m_oldx), m_time, m_autokeyframes, m_autotransition);
    m_item->adjustKeyframes("top", QVariant(m_oldy), m_time, m_autokeyframes, m_autotransition);
}

void MoveItemCommand::redo()
{
    m_item->setPos(m_x, m_y);
    m_item->adjustKeyframes("left", QVariant(m_x), m_time, m_autokeyframes, m_autotransition);
    m_item->adjustKeyframes("top", QVariant(m_y), m_time, m_autokeyframes, m_autotransition);
}


ResizeItemCommand::ResizeItemCommand(qreal width, qreal height, qreal oldwidth, qreal oldheight, AnimationScene *scene, ResizeableItem *item, QUndoCommand *parent)
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
    setText("Resize " + getItemTypeName(item));
}

void ResizeItemCommand::undo()
{
    m_item->setWidth(m_oldwidth);
    m_item->setHeight(m_oldheight);
    m_item->adjustKeyframes("width", QVariant(m_oldwidth), m_time, m_autokeyframes, m_autotransition);
    m_item->adjustKeyframes("height", QVariant(m_oldheight), m_time, m_autokeyframes, m_autotransition);
}

void ResizeItemCommand::redo()
{
    m_item->setWidth(m_width);
    m_item->setHeight(m_height);

    m_item->adjustKeyframes("width", QVariant(m_width), m_time, m_autokeyframes, m_autotransition);
    m_item->adjustKeyframes("height", QVariant(m_height), m_time, m_autokeyframes, m_autotransition);
}

ScaleItemCommand::ScaleItemCommand(qreal x, qreal y, qreal width, qreal height, qreal oldx, qreal oldy, qreal oldwidth, qreal oldheight, AnimationScene *scene, ResizeableItem *item, QUndoCommand *parent)
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
    setText("Scale " + getItemTypeName(item));
}

void ScaleItemCommand::undo()
{
    m_item->setPos(m_oldx, m_oldy);
    m_item->setWidth(m_oldwidth);
    m_item->setHeight(m_oldheight);
    m_item->adjustKeyframes("left", QVariant(m_oldx), m_time, m_autokeyframes, m_autotransition);
    m_item->adjustKeyframes("top", QVariant(m_oldy), m_time, m_autokeyframes, m_autotransition);
    m_item->adjustKeyframes("width", QVariant(m_oldwidth), m_time, m_autokeyframes, m_autotransition);
    m_item->adjustKeyframes("height", QVariant(m_oldheight), m_time, m_autokeyframes, m_autotransition);
    m_item->scaleObjects();
    m_item->posChanged(m_oldx, m_oldy);
}

void ScaleItemCommand::redo()
{
    m_item->setPos(m_x, m_y);
    m_item->setWidth(m_width);
    m_item->setHeight(m_height);
    m_item->adjustKeyframes("left", QVariant(m_x), m_time, m_autokeyframes, m_autotransition);
    m_item->adjustKeyframes("top", QVariant(m_y), m_time, m_autokeyframes, m_autotransition);
    m_item->adjustKeyframes("width", QVariant(m_width), m_time, m_autokeyframes, m_autotransition);
    m_item->adjustKeyframes("height", QVariant(m_height), m_time, m_autokeyframes, m_autotransition);
    m_item->scaleObjects();
    m_item->posChanged(m_x, m_y);
}

ChangeIdItemCommand::ChangeIdItemCommand(QString id, QString oldid, ResizeableItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_id = id;
    m_oldid = oldid;
    m_item = item;
    setText("Change " + getItemTypeName(item) + " Id");
}

void ChangeIdItemCommand::undo()
{
    m_item->setId(m_oldid);
}

void ChangeIdItemCommand::redo()
{
    m_item->setId(m_id);
}

ChangeColorItemCommand::ChangeColorItemCommand(QColor color, QColor oldcolor, ResizeableItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_color = color;
    m_oldcolor = oldcolor;
    m_item = item;
    setText("Change " + getItemTypeName(item) + " Color");
}

void ChangeColorItemCommand::undo()
{
    m_item->setBrush(QBrush(m_oldcolor));
}

void ChangeColorItemCommand::redo()
{
    m_item->setBrush(QBrush(m_color));
}

ChangePenItemCommand::ChangePenItemCommand(QColor color, QColor oldcolor, ResizeableItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_color = color;
    m_oldcolor = oldcolor;
    m_item = item;
    setText("Change " + getItemTypeName(item) + " Pen");
}

void ChangePenItemCommand::undo()
{
    m_item->setPen(QPen(m_oldcolor));
}

void ChangePenItemCommand::redo()
{
    m_item->setPen(QPen(m_color));
}

ChangeTextcolorItemCommand::ChangeTextcolorItemCommand(QColor color, QColor oldcolor, Text *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_color = color;
    m_oldcolor = oldcolor;
    m_item = item;
    setText("Change " + getItemTypeName(item) + " Textcolor");
}

void ChangeTextcolorItemCommand::undo()
{
    m_item->setTextcolor(m_oldcolor);
}

void ChangeTextcolorItemCommand::redo()
{
    m_item->setTextcolor(m_color);
}


