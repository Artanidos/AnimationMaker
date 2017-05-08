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

#include "resizeableitem.h"
#include "animationscene.h"
#include "keyframe.h"
#include "commands.h"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>
#include <QGuiApplication>

#include <QTest>
#include <QGraphicsScene>
#include <QMenu>

ResizeableItem::ResizeableItem(AnimationScene *scene)
{
    m_scene = scene;
    m_hasHandles = false;
    m_xscale = 1;
    m_yscale = 1;
    m_opacity = 100;
    m_keyframes = new QHash<QString, KeyFrame*>();

    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    delAct = new QAction(tr("Delete"), this);
    delAct->setShortcut(tr("Delete"));
    connect(delAct, SIGNAL(triggered()), this, SLOT(deleteItem()));

    bringToFrontAct = new QAction("Bring to front");
    connect(bringToFrontAct, SIGNAL(triggered()), this, SLOT(bringToFrontAction()));

    sendToBackAct = new QAction("Send to back");
    connect(sendToBackAct, SIGNAL(triggered()), this, SLOT(sendToBackAction()));

    raiseAct = new QAction("Raise");
    connect(raiseAct, SIGNAL(triggered()), this, SLOT(raiseAction()));

    lowerAct = new QAction("Lower");
    connect(lowerAct, SIGNAL(triggered()), this, SLOT(lowerAction()));

    m_contextMenu = new QMenu();
    m_contextMenu->addAction(delAct);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(bringToFrontAct);
    m_contextMenu->addAction(raiseAct);
    m_contextMenu->addAction(lowerAct);
    m_contextMenu->addAction(sendToBackAct);
    m_contextMenu->addSeparator();
}

ResizeableItem::~ResizeableItem()
{
    QHash<QString, KeyFrame*>::iterator it;
    for(it = m_keyframes->begin(); it != m_keyframes->end(); it++)
    {
        KeyFrame *frame = it.value();
        for(; frame != NULL; frame = frame->next())
        {
            if(frame->prev())
                delete frame->prev();
        }
        delete frame;
    }
    delete m_keyframes;
    delete delAct;
    delete bringToFrontAct;
    delete sendToBackAct;
    delete raiseAct;
    delete lowerAct;
    if(m_hasHandles)
    {
        for(int i = 0; i < 8; i++)
        {
            delete m_handles[i];
        }
    }
}

void ResizeableItem::addKeyframe(QString propertyName, KeyFrame *frame)
{
    if(m_keyframes->contains(propertyName))
    {
        KeyFrame * first = m_keyframes->value(propertyName);
        if(frame->time() < first->time())
        {
            first->setPrev(frame);
            frame->setNext(first);
            // first has changed, so update hash
            m_keyframes->remove(propertyName);
            m_keyframes->insert(propertyName, frame);
        }
        else
        {
            for(KeyFrame *f = first; f!= NULL; f=f->next())
            {
                if(f->next() == NULL)
                {
                    // append
                    f->setNext(frame);
                    frame->setPrev(f);
                    break;
                }
                else if(f->time() < frame->time() && f->next()->time() > frame->time())
                {
                    // insert
                    f->next()->setPrev(frame);
                    frame->setNext(f->next());
                    f->setNext(frame);
                    break;
                }
            }
        }
    }
    else
        m_keyframes->insert(propertyName, frame);
}

bool ResizeableItem::deleteKeyframe(QString propertyName, KeyFrame *frame)
{
    if(frame->next())
        frame->next()->setPrev(frame->prev());
    if(frame->prev())
    {
        frame->prev()->setEasing(-1);
        frame->prev()->setNext(frame->next());
    }
    else
    {
        m_keyframes->remove(propertyName);
        if(frame->next())
            m_keyframes->insert(propertyName, frame->next());
        else
            return true;
    }
    return false;
}

void ResizeableItem::drawHighlightSelected(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    qreal itemPenWidth = m_pen.widthF();
    const qreal pad = itemPenWidth / 2;
    const qreal penWidth = 0;
    const QColor fgcolor = option->palette.windowText().color();
    const QColor bgcolor( fgcolor.red()   > 127 ? 0 : 255, fgcolor.green() > 127 ? 0 : 255, fgcolor.blue()  > 127 ? 0 : 255);

    painter->setOpacity(1.0);
    painter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));

    painter->setPen(QPen(option->palette.windowText(), 0, Qt::DashLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));
}

QRectF ResizeableItem::rect() const
{
    return m_rect;
}

void ResizeableItem::scaleObjects() {}
void ResizeableItem::setScale(qreal x, qreal y)
{
    Q_UNUSED(x)
    Q_UNUSED(y)
}

qreal ResizeableItem::xscale()
{
    return m_xscale;
}

qreal ResizeableItem::yscale()
{
    return m_yscale;
}

void ResizeableItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    prepareGeometryChange();
    m_rect = QRectF(x, y, w, h);
    update();
    adjustKeyframes("width", QVariant(w), m_scene->playheadPosition(), m_scene->autokeyframes(), m_scene->autotransition());
    adjustKeyframes("height", QVariant(h), m_scene->playheadPosition(), m_scene->autokeyframes(), m_scene->autotransition());
    emit sizeChanged(w, h);
}

QString ResizeableItem::id() const
{
    return m_id;
}

void ResizeableItem::setId(const QString value)
{
    m_id = value;
    emit idChanged(this, value);
}

void ResizeableItem::setWidth(qreal value)
{
    prepareGeometryChange();
    m_rect.setWidth(value);
    update();
    setHandlePositions();
    emit sizeChanged(value, rect().height());
}

void ResizeableItem::setHeight(qreal value)
{
    prepareGeometryChange();
    m_rect.setHeight(value);
    update();
    setHandlePositions();
    emit sizeChanged(rect().width(), value);
}

QPen ResizeableItem::pen() const
{
    return m_pen;
}

void ResizeableItem::setPen(const QPen &pen)
{
    m_pen = pen;
    update();
    emit penChanged(m_pen.color());
}

QBrush ResizeableItem::brush() const
{
    return m_brush;
}
void ResizeableItem::setBrush(const QBrush &brush)
{
    m_brush = brush;
    update();
    emit brushChanged(m_brush.color());
}

void ResizeableItem::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(paint);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF ResizeableItem::boundingRect() const
{
    return rect();
}

bool ResizeableItem::sceneEventFilter(QGraphicsItem * watched, QEvent * event)
{
    ItemHandle * handle = dynamic_cast<ItemHandle *>(watched);
    if ( handle == NULL)
    {
        return false;
    }
    QGraphicsSceneMouseEvent * mevent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if ( mevent == NULL)
    {
        return false;
    }

    switch (event->type() )
    {
        case QEvent::GraphicsSceneMousePress:
        {
            m_oldx = this->pos().x();
            m_oldy = this->pos().y();
            m_oldwidth = this->rect().width();
            m_oldheight = this->rect().height();

            handle->setMouseState(ItemHandle::kMouseDown);
            handle->mouseDownX = mevent->pos().x();
            handle->mouseDownY = mevent->pos().y();
            break;
        }
        case QEvent::GraphicsSceneMouseRelease:
        {
            if(m_oldx != this->pos().x() || m_oldy != this->pos().y() || m_oldwidth != this->rect().width() || m_oldheight != this->rect().height())
            {
                QUndoStack *undoStack = m_scene->undoStack();
                QUndoCommand *cmd = new ScaleItemCommand(this->pos().x(), this->pos().y(), this->rect().width(), this->rect().height(), m_oldx, m_oldy, m_oldwidth, m_oldheight, m_scene, this);
                undoStack->push(cmd);
            }
            handle->setMouseState(ItemHandle::kMouseReleased);
            break;
        }
        case QEvent::GraphicsSceneMouseMove:
        {
            handle->setMouseState(ItemHandle::kMouseMoving );
            break;
        }
        default:
            return false;
    }

    if ( handle->getMouseState() == ItemHandle::kMouseMoving )
    {
        qreal x = mevent->pos().x(), y = mevent->pos().y();

        int XaxisSign = 0;
        int YaxisSign = 0;
        switch(handle->getCorner())
        {
            case 0:
            {
                XaxisSign = +1;
                YaxisSign = +1;
                break;
            }
            case 1:
            {
                XaxisSign = -1;
                YaxisSign = +1;
                break;
            }
            case 2:
            {
                XaxisSign = -1;
                YaxisSign = -1;
                break;
            }
            case 3:
            {
                XaxisSign = +1;
                YaxisSign = -1;
                break;
            }
            case 4:
            {
                YaxisSign = +1;
                break;
            }
            case 5:
            {
                XaxisSign = -1;
                break;
            }
            case 6:
            {
                YaxisSign = -1;
                break;
            }
            case 7:
            {
                XaxisSign = +1;
                break;
            }
        }

        int xMoved = handle->mouseDownX - x;
        int yMoved = handle->mouseDownY - y;

        int newWidth = rect().width() + ( XaxisSign * xMoved);
        if ( newWidth < 20 ) newWidth  = 20;

        int newHeight = rect().height() + (YaxisSign * yMoved);
        if ( newHeight < 20 ) newHeight = 20;

        qreal deltaWidth = newWidth - rect().width();
        qreal deltaHeight = newHeight - rect().height();

        bool controlPressed = QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier);
        if(controlPressed)
        {
            // keep ratio
            qreal ratio = rect().width() / rect().height();
            if(handle->getCorner() < 4) // corners
            {
                if(newWidth > newHeight)
                    deltaWidth = (int)(deltaHeight * ratio);
                else
                    deltaHeight = (int)(deltaWidth / ratio);
            }
            else
            {
                if(handle->getCorner() == 4 || handle->getCorner() == 6) // top | bottom
                    deltaWidth = deltaHeight * ratio;
                else // left | right
                    deltaHeight = deltaWidth / ratio;
            }
        }

        setRect(0,0,rect().width() + deltaWidth, rect().height() + deltaHeight);
        adjustKeyframes("width", QVariant(rect().width() + deltaWidth), m_scene->playheadPosition(), m_scene->autokeyframes(), m_scene->autotransition());
        adjustKeyframes("height", QVariant(rect().height() + deltaHeight), m_scene->playheadPosition(), m_scene->autokeyframes(), m_scene->autotransition());
        scaleObjects();

        deltaWidth *= (-1);
        deltaHeight *= (-1);

        bool shiftPressed = QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier);

        qreal newXpos = this->pos().x();
        qreal newYpos = this->pos().y();

        switch(handle->getCorner())
        {
            case 0: // top left
            {
                if(shiftPressed)
                {
                    newXpos = this->pos().x() + deltaWidth / 2;
                    newYpos = this->pos().y() + deltaHeight / 2;
                }
                else
                {
                    newXpos = this->pos().x() + deltaWidth;
                    newYpos = this->pos().y() + deltaHeight;
                }
                break;
            }
            case 1: // top right
            {
                if(shiftPressed)
                {
                    newXpos = this->pos().x() + deltaWidth / 2;
                    newYpos = this->pos().y() + deltaHeight / 2;
                }
                else
                    newYpos = this->pos().y() + deltaHeight;
                break;
            }
            case 2: // bottom right
            {
                if(shiftPressed)
                {
                    newXpos = this->pos().x() + deltaWidth / 2;
                    newYpos = this->pos().y() + deltaHeight / 2;
                }
                break;
            }
            case 3: // bottom left
            {
                if(shiftPressed)
                {
                    newXpos = this->pos().x() + deltaWidth / 2;
                    newYpos = this->pos().y() + deltaHeight / 2;
                }
                else
                    newXpos = this->pos().x() + deltaWidth;
                break;
            }
            case 4: // top
            {
                if(shiftPressed)
                {
                    newXpos = this->pos().x() + deltaWidth / 2;
                    newYpos = this->pos().y() + deltaHeight / 2;
                }
                else if(controlPressed)
                {
                    newYpos = this->pos().y() + deltaHeight;
                    newXpos = this->pos().x() + deltaWidth / 2;
                }
                else
                {
                    newYpos = this->pos().y() + deltaHeight;
                }
                break;
            }
            case 5: // right
            {
                if(shiftPressed)
                {
                    newXpos = this->pos().x() + deltaWidth / 2;
                    newYpos = this->pos().y() + deltaHeight / 2;
                }
                else if(controlPressed)
                {
                    newYpos = this->pos().y() + deltaHeight / 2;
                }
                break;
            }
            case 6: // bottom
            {
                if(shiftPressed)
                {
                    newXpos = this->pos().x() + deltaWidth / 2;
                    newYpos = this->pos().y() + deltaHeight / 2;
                }
                else if(controlPressed)
                {
                    newXpos = this->pos().x() + deltaWidth / 2;
                }
                break;
            }
            case 7: // left
            {
                if(shiftPressed)
                {
                    newXpos = this->pos().x() + deltaWidth / 2;
                    newYpos = this->pos().y() + deltaHeight / 2;
                }
                else if(controlPressed)
                {
                    newXpos = this->pos().x() + deltaWidth;
                    newYpos = this->pos().y() + deltaHeight / 2;
                }
                else
                {
                    newXpos = this->pos().x() + deltaWidth;
                }
                break;
            }
        }
        if(newXpos != this->pos().x() || newYpos != this->pos().y())
        {
            this->setPos(newXpos, newYpos);
            posChanged(newXpos, newYpos);
        }
        setHandlePositions();

        this->update();
    }
    return true;
}

void ResizeableItem::setHandlePositions()
{
    if(!m_hasHandles)
        return;
    m_handles[0]->setPos(x() - 4, y() - 4);
    m_handles[1]->setPos(x() + rect().width() - 4, y() - 4);
    m_handles[2]->setPos(x() + rect().width() - 4, y() + rect().height() - 4);
    m_handles[3]->setPos(x() - 4, y() + rect().height() - 4);
    m_handles[4]->setPos(x() + rect().width() / 2 - 4, y() - 4);
    m_handles[5]->setPos(x() + rect().width() - 4, y() + rect().height() / 2 - 4);
    m_handles[6]->setPos(x() + rect().width() /2 - 4, y() + rect().height() - 4);
    m_handles[7]->setPos(x() - 4, y() + rect().height() / 2 - 4);
}

QVariant ResizeableItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
    {
        if (value == true)
        {
            if(!m_hasHandles)
            {
                for(int i = 0; i < 8; i++)
                {
                    m_handles[i] = new ItemHandle(i);
                    this->scene()->addItem(m_handles[i]);
                    m_handles[i]->installSceneEventFilter(this);
                }
                m_hasHandles = true;
                setHandlePositions();            
            }
        }
        else
        {
            for(int i = 0; i < 8; i++)
            {
                delete m_handles[i];
            }
            m_hasHandles = false;
        }
    }
    else if(change == QGraphicsItem::ItemPositionHasChanged)
    {
        if(isSelected())
        {
            QPointF newPos = value.toPointF();
            posChanged(newPos.x(), newPos.y());
            setHandlePositions();
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void ResizeableItem::posChanged(qreal x, qreal y)
{
    adjustKeyframes("left", QVariant(x), m_scene->playheadPosition(), m_scene->autokeyframes(), m_scene->autotransition());
    adjustKeyframes("top", QVariant(y), m_scene->playheadPosition(), m_scene->autokeyframes(), m_scene->autotransition());
    emit positionChanged(x, y);
}

/*
 * Looking for the keyframe which occures in front of the playhead position
 * and adjust its value.
 * If no keyframe will be found and autokeyframe is switched to on a new keyframe will be added
 */
void ResizeableItem::adjustKeyframes(QString propertyName, QVariant value, int time, bool autokeyframes, bool autotransition)
{
    if(m_keyframes->contains(propertyName))
    {
        KeyFrame *first = m_keyframes->value(propertyName);
        KeyFrame *found = NULL;
        KeyFrame *last = NULL;
        for(KeyFrame *frame = first; frame != NULL; frame = frame->next())
        {
            if(autokeyframes)
            {
                if(frame->time() < time)
                    last = frame;
                if(frame->time() == time)
                    found = frame;
            }
            else
            {
                if(frame->time() <= time)
                    found = frame;
            }
        }
        if(found)
        {
            found->setValue(value);
        }
        else
        {
            if(autokeyframes)
            {
                KeyFrame *newFrame = new KeyFrame();
                newFrame->setValue(value);
                newFrame->setTime(time);
                if(last == NULL)
                {
                    newFrame->setNext(first);
                    first->setPrev(newFrame);
                    // first has changed, so update hash
                    m_keyframes->remove(propertyName);
                    m_keyframes->insert(propertyName, newFrame);

                    if(autotransition)
                        newFrame->setEasing(QEasingCurve::Linear);
                }
                else
                {
                    newFrame->setPrev(last);
                    if(last->next())
                    {
                        newFrame->setNext(last->next());
                        last->next()->setPrev(newFrame);
                    }
                    last->setNext(newFrame);

                    if(autotransition)
                        last->setEasing(QEasingCurve::Linear);
                }
            }
        }
    }
}

void ResizeableItem::setOpacity(int opacity)
{
    m_opacity = opacity;
    QGraphicsItem::setOpacity((qreal)m_opacity / 100);
    emit opacityChanged(opacity);
}

void ResizeableItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    m_contextMenu->exec(event->screenPos());
}

void ResizeableItem::lowerAction()
{
    QUndoCommand *cmd = new LowerItemCommand(this);
    m_scene->undoStack()->push(cmd);
}

void ResizeableItem::raiseAction()
{
    QUndoCommand *cmd = new RaiseItemCommand(this);
    m_scene->undoStack()->push(cmd);
}

void ResizeableItem::sendToBackAction()
{
    QUndoCommand *cmd = new SendItemToBackCommand(this);
    m_scene->undoStack()->push(cmd);
}

void ResizeableItem::bringToFrontAction()
{
    QUndoCommand *cmd = new BringItemToFrontCommand(this);
    m_scene->undoStack()->push(cmd);
}

void ResizeableItem::lower()
{
    int pos = scene()->items().indexOf(this);
    for(int i = pos + 1; i < scene()->items().count(); i++)
    {
        QGraphicsItem *item = scene()->items().at(i);
        if(isAnimationMakerItem(item))
        {
            this->stackBefore(item);
            break;
        }
    }
    // trick to repaint item
    this->setSelected(false);
    this->setSelected(true);
}

void ResizeableItem::raise()
{
    int pos = scene()->items().indexOf(this);
    for(int i = pos - 1; i >= 0; i--)
    {
        QGraphicsItem *item = scene()->items().at(i);
        if(isAnimationMakerItem(item))
        {
            item->stackBefore(this);
            break;
        }
    }
    // trick to repaint item
    this->setSelected(false);
    this->setSelected(true);
}

void ResizeableItem::bringToFront()
{
    int pos = scene()->items().indexOf(this);
    for(int i = pos - 1; i >= 0; i--)
    {
        QGraphicsItem *item = scene()->items().at(i);
        if(isAnimationMakerItem(item))
        {
            item->stackBefore(this);
        }
    }
    // trick to repaint item
    this->setSelected(false);
    this->setSelected(true);
}

void ResizeableItem::sendToBack()
{
    int pos = scene()->items().indexOf(this);
    for(int i = pos + 1; i < scene()->items().count(); i++)
    {
        QGraphicsItem *item = scene()->items().at(i);
        if(isAnimationMakerItem(item))
        {
            this->stackBefore(item);
        }
    }
    // trick to repaint item
    this->setSelected(false);
    this->setSelected(true);
}

void ResizeableItem::deleteItem()
{
    m_scene->deleteItem(this);
}
