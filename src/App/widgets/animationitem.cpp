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

#include "animationitem.h"

#include <QEasingCurve>
#include <QGuiApplication>
#include <QUndoStack>

#include "commands.h"

AnimationItem::AnimationItem(AnimationScene *scene, bool isSceneRect)
{
    m_scene = scene;
    m_isSceneRect = isSceneRect;
    m_deleted = false;
    m_hasHandles = false;
    m_xscale = 1;
    m_yscale = 1;
    m_scaleX = 1.0;
    m_scaleY = 1.0;
    m_shearX = 0.0;
    m_shearY = 0.0;
    m_rotationX = 0;
    m_rotationY = 0;
    m_rotationZ = 0;
    m_originX = 0;
    m_originY = 0;
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

AnimationItem::~AnimationItem()
{
    QHash<QString, KeyFrame*>::iterator it;
    for(it = m_keyframes->begin(); it != m_keyframes->end(); it++)
    {
        KeyFrame *frame = it.value();
        for(; frame != nullptr; frame = frame->next())
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

void AnimationItem::setScene(AnimationScene *scene)
{
    m_scene = scene;
}

void AnimationItem::addKeyframe(QString propertyName, KeyFrame *frame)
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
            for(KeyFrame *f = first; f!= nullptr; f=f->next())
            {
                if(f->next() == nullptr)
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

bool AnimationItem::deleteKeyframe(QString propertyName, KeyFrame *frame)
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

void AnimationItem::drawHighlightSelected(QPainter *painter, const QStyleOptionGraphicsItem *option)
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

QRectF AnimationItem::rect() const
{
    return m_rect;
}

void AnimationItem::scaleObjects() {}
void AnimationItem::setScale(qreal x, qreal y)
{
    Q_UNUSED(x)
    Q_UNUSED(y)
}

qreal AnimationItem::xscale()
{
    return m_xscale;
}

qreal AnimationItem::yscale()
{
    return m_yscale;
}

void AnimationItem::doTransform()
{
    QTransform t;
    t.translate(m_originX, m_originY);
    t.scale(m_scaleX, m_scaleY);
    t.shear(m_shearX, m_shearY);
    t.rotate(m_rotationX, Qt::XAxis);
    t.rotate(m_rotationY, Qt::YAxis);
    t.rotate(m_rotationZ, Qt::ZAxis);
    t.translate(-m_originX, -m_originY);
    setTransform(t, false);
}

void AnimationItem::setScaleX(double value)
{
    m_scaleX = value;
    doTransform();
    emit scaleXChanged(this, value);
}

void AnimationItem::setScaleY(double value)
{
    m_scaleY = value;
    doTransform();
    emit scaleYChanged(this, value);
}

void AnimationItem::setShearX(double value)
{
    m_shearX = value;
    doTransform();
    emit shearXChanged(this, value);
}

void AnimationItem::setShearY(double value)
{
    m_shearY = value;
    doTransform();
    emit shearYChanged(this, value);
}

void AnimationItem::setRotationX(int value)
{
    m_rotationX = value;
    doTransform();
    emit rotationXChanged(this, value);
}

void AnimationItem::setRotationY(int value)
{
    m_rotationY = value;
    doTransform();
    emit rotationYChanged(this, value);
}

void AnimationItem::setRotationZ(int value)
{
    m_rotationZ = value;
    doTransform();
    emit rotationZChanged(this, value);
}

void AnimationItem::setOriginX(int value)
{
    m_originX = value;
    doTransform();
    emit originXChanged(this, value);
}

void AnimationItem::setOriginY(int value)
{
    m_originY = value;
    doTransform();
    emit originYChanged(this, value);
}

void AnimationItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    prepareGeometryChange();
    m_rect = QRectF(x, y, w, h);
    update();
    if(m_scene)
    {
       emit sizeChanged(w, h);
    }
}

QString AnimationItem::id() const
{
    return m_id;
}

void AnimationItem::setId(const QString value)
{
    m_id = value;
    emit idChanged(this, value);
}

void AnimationItem::setWidth(qreal value)
{
    prepareGeometryChange();
    m_rect.setWidth(value);
    scaleObjects();
    update();
    setHandlePositions();
    emit sizeChanged(value, rect().height());
}

void AnimationItem::setHeight(qreal value)
{
    prepareGeometryChange();
    m_rect.setHeight(value);
    scaleObjects();
    update();
    setHandlePositions();
    emit sizeChanged(rect().width(), value);
}

QPen AnimationItem::pen() const
{
    return m_pen;
}

void AnimationItem::setPen(const QPen &pen)
{
    m_pen = pen;
    update();
    emit penChanged(m_pen.color());
}

QBrush AnimationItem::brush() const
{
    return m_brush;
}

void AnimationItem::setBrush(const QBrush &brush)
{
    m_brush = brush;
    update();
}

QColor AnimationItem::brushColor() const
{
    return m_brush.color();
}

void AnimationItem::setBrushColor(const QColor &color)
{
    m_brush.setColor(color);
    update();
    emit brushChanged(m_brush.color());
}

QColor AnimationItem::penColor() const
{
    return m_pen.color();
}

void AnimationItem::setPenColor(const QColor &color)
{
    m_pen.setColor(color);
    update();
    emit penChanged(m_pen.color());
}

void AnimationItem::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(paint);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF AnimationItem::boundingRect() const
{
    return rect();
}

bool AnimationItem::sceneEventFilter(QGraphicsItem * watched, QEvent * event)
{
    ItemHandle * handle = dynamic_cast<ItemHandle *>(watched);
    if ( handle == nullptr)
    {
        return false;
    }
    QGraphicsSceneMouseEvent * mevent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if ( mevent == nullptr)
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

void AnimationItem::setHandlePositions()
{
    qreal halfwidth;
    if(!m_hasHandles)
        return;

    halfwidth = m_handles[0]->width() / 2.0;
    m_handles[0]->setPos(-halfwidth, -halfwidth);
    m_handles[1]->setPos(rect().width() - halfwidth, -halfwidth);
    m_handles[2]->setPos(rect().width() - halfwidth, rect().height() - halfwidth);
    m_handles[3]->setPos(-halfwidth, rect().height() - halfwidth);
    m_handles[4]->setPos(rect().width() / 2 - halfwidth, -halfwidth);
    m_handles[5]->setPos(rect().width() - halfwidth, rect().height() / 2 - halfwidth);
    m_handles[6]->setPos(rect().width() /2 - halfwidth, rect().height() - halfwidth);
    m_handles[7]->setPos(- halfwidth, rect().height() / 2 - halfwidth);

    m_scene->update(x() - halfwidth - 5, y() - halfwidth - 5, x() + rect().width() + halfwidth * 2 + 5, y() + rect().height() + halfwidth * 2 + 5);
}

QVariant AnimationItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
    {
        if (value == true)
        {
            if(!m_hasHandles)
            {
                for(int i = 0; i < 8; i++)
                {
                    m_handles[i] = new ItemHandle(this, i, scene()->scaling());
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

void AnimationItem::posChanged(qreal x, qreal y)
{
    emit positionChanged(x, y);
}

/*
 * Looking for the keyframe which occures in front of the playhead position
 * and adjust its value.
 * If no keyframe will be found and autokeyframe is switched to on a new keyframe will be added
 */
void AnimationItem::adjustKeyframes(QString propertyName, QVariant value, int time, bool autokeyframes, bool autotransition, KeyFrame **key, bool undo)
{
    if(m_keyframes->contains(propertyName))
    {
        if(undo && (*key) != nullptr)
        {
            if(autotransition)
            {
                if((*key)->prev()->easing() > -1)
                {
                    (*key)->prev()->setEasing(-1);
                    emit transitionRemoved(propertyName, (*key)->prev());
                }
            }
            // we have to drop the newest keyframe here while undoing
            if((*key)->next())
            {
                (*key)->next()->setPrev((*key)->prev());
            }
            if((*key)->prev())
            {
                (*key)->prev()->setNext((*key)->next());
            }
            emit keyframeRemoved(propertyName, (*key));
            delete (*key);
            (*key) = nullptr;
        }

        KeyFrame *first = m_keyframes->value(propertyName);
        KeyFrame *found = nullptr;
        KeyFrame *last = nullptr;
        for(KeyFrame *frame = first; frame != nullptr; frame = frame->next())
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
            if(autokeyframes && !undo)
            {
                *key = new KeyFrame();
                (*key)->setValue(value);
                (*key)->setTime(time);
                if(last == nullptr)
                {
                    (*key)->setNext(first);
                    first->setPrev((*key));
                    // first has changed, so update hash
                    m_keyframes->remove(propertyName);
                    m_keyframes->insert(propertyName, (*key));

                    if(autotransition)
                        (*key)->setEasing(QEasingCurve::Linear);
                }
                else
                {
                    (*key)->setPrev(last);
                    if(last->next())
                    {
                        (*key)->setNext(last->next());
                        last->next()->setPrev((*key));
                    }
                    last->setNext((*key));

                    if(autotransition)
                        last->setEasing(QEasingCurve::Linear);
                }
                emit keyframeAdded(propertyName, (*key));
            }
        }
    }
}

void AnimationItem::setOpacity(int opacity)
{
    m_opacity = opacity;
    QGraphicsItem::setOpacity((qreal)m_opacity / 100);
    emit opacityChanged(opacity);
}

void AnimationItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(!m_isSceneRect)
    {
        scene()->clearSelection();
        setSelected(true);
        m_contextMenu->exec(event->screenPos());
    }
}

void AnimationItem::lowerAction()
{
    QUndoCommand *cmd = new LowerItemCommand(this);
    m_scene->undoStack()->push(cmd);
}

void AnimationItem::raiseAction()
{
    QUndoCommand *cmd = new RaiseItemCommand(this);
    m_scene->undoStack()->push(cmd);
}

void AnimationItem::sendToBackAction()
{
    QUndoCommand *cmd = new SendItemToBackCommand(this);
    m_scene->undoStack()->push(cmd);
}

void AnimationItem::bringToFrontAction()
{
    QUndoCommand *cmd = new BringItemToFrontCommand(this);
    m_scene->undoStack()->push(cmd);
}

void AnimationItem::lower()
{
    int pos = scene()->items().indexOf(this);
    for(int i = pos + 1; i < scene()->items().count(); i++)
    {
        AnimationItem *item = dynamic_cast<AnimationItem*>(scene()->items().at(i));
        if(item)
        {
            this->stackBefore(item);
            break;
        }
    }
    // trick to repaint item
    this->setSelected(false);
    this->setSelected(true);
}

void AnimationItem::raise()
{
    int pos = scene()->items().indexOf(this);
    for(int i = pos - 1; i >= 0; i--)
    {
        AnimationItem *item = dynamic_cast<AnimationItem*>(scene()->items().at(i));
        if(item)
        {
            item->stackBefore(this);
            break;
        }
    }
    // trick to repaint item
    this->setSelected(false);
    this->setSelected(true);
}

void AnimationItem::bringToFront()
{
    int pos = scene()->items().indexOf(this);
    for(int i = pos - 1; i >= 0; i--)
    {
        AnimationItem *item = dynamic_cast<AnimationItem*>(scene()->items().at(i));
        if(item)
        {
            item->stackBefore(this);
        }
    }
    // trick to repaint item
    this->setSelected(false);
    this->setSelected(true);
}

void AnimationItem::sendToBack()
{
    int pos = scene()->items().indexOf(this);
    for(int i = pos + 1; i < scene()->items().count(); i++)
    {
        AnimationItem *item = dynamic_cast<AnimationItem*>(scene()->items().at(i));
        if(item)
        {
            this->stackBefore(item);
        }
    }
    // trick to repaint item
    this->setSelected(false);
    this->setSelected(true);
}

void AnimationItem::writeAttributes(QDomElement ele)
{
    ele.setAttribute("id", id());
    ele.setAttribute("left", QVariant(left()).toString());
    ele.setAttribute("top", QVariant(top()).toString());
    ele.setAttribute("width", QVariant(rect().width()).toString());
    ele.setAttribute("height", QVariant(rect().height()).toString());
    ele.setAttribute("opacity", opacity());
    ele.setAttribute("scalex", QVariant(scaleX()).toString());
    ele.setAttribute("scaley", QVariant(scaleY()).toString());
    ele.setAttribute("shearx", QVariant(shearX()).toString());
    ele.setAttribute("sheary", QVariant(shearY()).toString());
    ele.setAttribute("rotationx", QVariant(rotationX()).toString());
    ele.setAttribute("rotationy", QVariant(rotationY()).toString());
    ele.setAttribute("rotationz", QVariant(rotationZ()).toString());
    ele.setAttribute("originx", QVariant(originX()).toString());
    ele.setAttribute("originy", QVariant(originY()).toString());
    ele.setAttribute("visible", isVisible() ? "true" : "false");
    ele.setAttribute("locked", this->flags().testFlag(QGraphicsItem::ItemIsSelectable) ? "false" : "true");
}

void AnimationItem::readAttributes(QDomElement ele)
{
    setId(ele.attribute("id"));
    setLeft(ele.attribute("left", "0").toDouble());
    setTop(ele.attribute("top", "0").toDouble());
    setWidth(ele.attribute("width", "50").toDouble());
    setHeight(ele.attribute("height", "50").toDouble());
    setOpacity(ele.attribute("opacity", "100").toInt());
    setScaleX(ele.attribute("scalex", "1").toDouble());
    setScaleY(ele.attribute("scaley", "1").toDouble());
    setShearX(ele.attribute("shearx", "0").toDouble());
    setShearY(ele.attribute("sheary", "0").toDouble());
    setRotationX(ele.attribute("rotationx", "0").toInt());
    setRotationY(ele.attribute("rotationy", "0").toInt());
    setRotationZ(ele.attribute("rotationz", "0").toInt());
    setOriginX(ele.attribute("originx", "0").toInt());
    setOriginY(ele.attribute("originy", "0").toInt());
    setVisible(ele.attribute("visible", "true") == "true");
    setFlag(QGraphicsItem::ItemIsSelectable, ele.attribute("locked", "false") == "false");
    setFlag(QGraphicsItem::ItemIsMovable, ele.attribute("locked", "false") == "false");
}

void AnimationItem::deleteItem()
{
    m_scene->deleteItem(this);
}
