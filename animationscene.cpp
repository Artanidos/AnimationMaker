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

#include "animationscene.h"
#include "rectangle.h"
#include "ellipse.h"
#include "text.h"
#include "bitmap.h"
#include "vectorgraphic.h"
#include "keyframe.h"
#include "commands.h"
#include <QUndoCommand>

AnimationScene::AnimationScene()
{
    m_autokeyframes = false;
    m_autotransitions = false;
    initialize();
}

void AnimationScene::initialize()
{
    setSceneRect(0, 0, 1200, 720);
    m_editMode = EditMode::ModeSelect;
    m_fps = 24;
    m_copy = NULL;
    m_playheadPosition = 0;
    m_tempKeyFrame = NULL;
    m_movingItem = NULL;

    addBackgroundRect();
}

void AnimationScene::reset()
{
    clear();
    initialize();
    m_undoStack->clear();
}

void AnimationScene::deleteItem(ResizeableItem *item)
{
    QUndoCommand *deleteCommand = new DeleteItemCommand(item, this);
    m_undoStack->push(deleteCommand);
}

void AnimationScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    if(m_editMode == EditMode::ModeSelect)
    {
        QPointF mousePos(mouseEvent->buttonDownScenePos(Qt::LeftButton).x(), mouseEvent->buttonDownScenePos(Qt::LeftButton).y());
        const QList<QGraphicsItem *> itemList = items(mousePos);
        m_movingItem = itemList.isEmpty() ? 0 : dynamic_cast<ResizeableItem*>(itemList.first());
        if(m_movingItem)
            m_oldPos = m_movingItem->pos();
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
    else
    {
        QString fileName;
        QString filter;
        QString title;
        if(m_editMode == EditMode::ModeBitmap)
        {
            filter = "Image Files (*.png *.jpeg *.jpg *.gif *.bmp);;All Files (*)";
            title = "Open Bitmap";
        }
        else if(m_editMode == EditMode::ModeSvg)
        {
            filter = "SVG Files (*.svg);;All Files (*)";
            title = "Open SVG";
        }
        if(!filter.isEmpty())
        {
            QFileDialog *dialog = new QFileDialog();
            dialog->setFileMode(QFileDialog::AnyFile);
            dialog->setNameFilter(filter);
            dialog->setWindowTitle(title);
            dialog->setOption(QFileDialog::DontUseNativeDialog, true);
            dialog->setAcceptMode(QFileDialog::AcceptOpen);
            if(dialog->exec())
                fileName = dialog->selectedFiles().first();
            delete dialog;
            if(fileName.isEmpty())
                return;
        }
        QUndoCommand *addCommand = new AddItemCommand(mouseEvent->scenePos().x(), mouseEvent->scenePos().y(), m_editMode, fileName, this);
        m_undoStack->push(addCommand);
    }
}

void AnimationScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void AnimationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(m_movingItem && mouseEvent->button() == Qt::LeftButton)
    {
        if(m_oldPos != m_movingItem->pos())
        {
            QUndoCommand *cmd = new MoveItemCommand(m_movingItem->x(), m_movingItem->y(), m_oldPos.x(), m_oldPos.y(), this, m_movingItem);
            m_undoStack->push(cmd);
        }
        m_movingItem = NULL;
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void AnimationScene::setEditMode(EditMode mode)
{
    m_editMode = mode;
}

void AnimationScene::addBackgroundRect()
{
    m_rect = new QGraphicsRectItem(0, 0, width(), height());
    m_backgroundColor = QColor("#404244");
    m_rect->setBrush(QBrush(QColor(m_backgroundColor)));
    m_rect->setPos(0,0);
    addItem(m_rect);
}

void AnimationScene::readKeyframes(QDataStream &dataStream, ResizeableItem *item)
{
    int vars, easing, time, keyframes;
    QVariant value;
    QString propertyName;

    m_tempKeyFrame = NULL;
    dataStream >> vars;
    qDebug() << "read" << vars;

    for(int i=0; i < vars; i++)
    {
        dataStream >> propertyName;
        dataStream >> keyframes;
        for(int j=0; j < keyframes; j++)
        {
            dataStream >> time;
            dataStream >> value;
            dataStream >> easing;
            KeyFrame *key = new KeyFrame();
            key->setTime(time);
            key->setValue(value);
            key->setEasing(easing);
            item->addKeyframe(propertyName, key);
            // set double linked list
            if(m_tempKeyFrame)
            {
                m_tempKeyFrame->setNext(key);
                key->setPrev(m_tempKeyFrame);
            }
            m_tempKeyFrame = key;
            emit keyframeAdded(item, propertyName, key);
        }
    }
}

QDataStream& AnimationScene::read(QDataStream &dataStream)
{
    int type, fps, opacity;
    qreal x, y, width, height, xscale, yscale;
    QPen pen;
    QBrush brush;
    QString text, id;
    QColor color, bgColor;

    clear();

    addBackgroundRect();

    dataStream >> width;
    dataStream >> height;
    dataStream >> fps;
    dataStream >> bgColor;

    this->setWidth(width);
    this->setHeight(height);
    this->setFps(fps);
    this->setBackgroundColor(bgColor);

    while (!dataStream.atEnd())
    {
        dataStream >> type;
        if(type == Rectangle::Type)
        {
            dataStream >> id;
            dataStream >> x;
            dataStream >> y;
            dataStream >> width;
            dataStream >> height;
            dataStream >> pen;
            dataStream >> brush;
            dataStream >> opacity;

            Rectangle *r = new Rectangle(width, height, this);
            r->setId(id);
            r->setPos(x, y);
            r->setPen(pen);
            r->setBrush(brush);
            r->setFlag(QGraphicsItem::ItemIsMovable, true);
            r->setFlag(QGraphicsItem::ItemIsSelectable, true);
            r->setOpacity(opacity);

            readKeyframes(dataStream, r);

            addItem(r);
        }
        else if(type == Ellipse::Type)
        {
            dataStream >> id;
            dataStream >> x;
            dataStream >> y;
            dataStream >> width;
            dataStream >> height;
            dataStream >> pen;
            dataStream >> brush;
            dataStream >> opacity;

            Ellipse *e = new Ellipse(width, height, this);
            e->setId(id);
            e->setPos(x, y);
            e->setPen(pen);
            e->setBrush(brush);
            e->setFlag(QGraphicsItem::ItemIsMovable, true);
            e->setFlag(QGraphicsItem::ItemIsSelectable, true);
            e->setOpacity(opacity);

            readKeyframes(dataStream, e);

            addItem(e);
        }
        else if(type == Text::Type)
        {
            dataStream >> id;
            dataStream >> x;
            dataStream >> y;
            dataStream >> xscale;
            dataStream >> yscale;
            dataStream >> text;
            dataStream >> color;
            dataStream >> opacity;

            Text *t = new Text(text, this);
            t->setId(id);
            t->setPos(x, y);
            t->setFlag(QGraphicsItem::ItemIsMovable, true);
            t->setFlag(QGraphicsItem::ItemIsSelectable, true);
            t->setScale(xscale, yscale);
            t->setTextcolor(color);
            t->setOpacity(opacity);

            readKeyframes(dataStream, t);

            addItem(t);
        }
        else if(type == Bitmap::Type)
        {
            QImage img;
            dataStream >> id;
            dataStream >> x;
            dataStream >> y;
            dataStream >> width;
            dataStream >> height;
            dataStream >> img;
            dataStream >> opacity;

            Bitmap *b = new Bitmap(img, width, height, this);
            b->setId(id);
            b->setPos(x, y);
            b->setFlag(QGraphicsItem::ItemIsMovable, true);
            b->setFlag(QGraphicsItem::ItemIsSelectable, true);
            b->setScale(xscale, yscale);
            b->setOpacity(opacity);

            readKeyframes(dataStream, b);

            addItem(b);
        }
        else if(type == Vectorgraphic::Type)
        {
            QByteArray arr;
            dataStream >> id;
            dataStream >> x;
            dataStream >> y;
            dataStream >> xscale;
            dataStream >> yscale;
            dataStream >> opacity;
            dataStream >> arr;
            Vectorgraphic *v = new Vectorgraphic(arr, this);
            v->setId(id);
            v->setPos(x, y);
            v->setFlag(QGraphicsItem::ItemIsMovable, true);
            v->setFlag(QGraphicsItem::ItemIsSelectable, true);
            v->setScale(xscale, yscale);
            v->setOpacity(opacity);

            readKeyframes(dataStream, v);

            addItem(v);
        }
    }

    return dataStream;
}

void AnimationScene::writeKeyframes(QDataStream &dataStream, ResizeableItem *item) const
{
    qDebug() << "write" << item->keyframes()->count();
    dataStream << item->keyframes()->count();
    QHash<QString, KeyFrame*>::iterator it;
    for(it = item->keyframes()->begin(); it != item->keyframes()->end(); it++)
    {
        dataStream << it.key();
        KeyFrame *first = it.value();
        int count = 0;
        for(KeyFrame *frame = first; frame != NULL; frame = frame->next())
            count++;

        dataStream << count;
        for(KeyFrame *frame = first; frame != NULL; frame = frame->next())
        {
            dataStream << frame->time();
            dataStream << frame->value();
            dataStream << frame->easing();
        }
    }
}

QDataStream& AnimationScene::write(QDataStream &dataStream) const
{
    dataStream << width();
    dataStream << height();
    dataStream << fps();
    dataStream << backgroundColor();

    QList<QGraphicsItem*> itemList = items(Qt::AscendingOrder);
    foreach (QGraphicsItem *item, itemList)
    {
        switch(item->type())
        {
            case Rectangle::Type:
            {
                Rectangle *r = dynamic_cast<Rectangle *>(item);
                dataStream << Rectangle::Type;
                dataStream << r->id();
                dataStream << r->pos().x();
                dataStream << r->pos().y();
                dataStream << r->rect().width();
                dataStream << r->rect().height();
                dataStream << r->pen();
                dataStream << r->brush();
                dataStream << r->opacity();

                writeKeyframes(dataStream, r);
                break;
            }
            case Ellipse::Type:
            {
                Ellipse *e = dynamic_cast<Ellipse *>(item);
                dataStream << Ellipse::Type;
                dataStream << e->id();
                dataStream << e->pos().x();
                dataStream << e->pos().y();
                dataStream << e->rect().width();
                dataStream << e->rect().height();
                dataStream << e->pen();
                dataStream << e->brush();
                dataStream << e->opacity();

                writeKeyframes(dataStream, e);
                break;
            }
            case Text::Type:
            {
                Text *t = dynamic_cast<Text *>(item);
                dataStream << Text::Type;
                dataStream << t->id();
                dataStream << t->pos().x();
                dataStream << t->pos().y();
                dataStream << t->xscale();
                dataStream << t->yscale();
                dataStream << t->text();
                dataStream << t->textcolor();
                dataStream << t->opacity();

                writeKeyframes(dataStream, t);
                break;
            }
            case Bitmap::Type:
            {
                Bitmap *b = dynamic_cast<Bitmap *>(item);
                dataStream << Bitmap::Type;
                dataStream << b->id();
                dataStream << b->pos().x();
                dataStream << b->pos().y();
                dataStream << b->rect().width();
                dataStream << b->rect().height();
                dataStream << b->getImage();
                dataStream << b->opacity();

                writeKeyframes(dataStream, b);
                break;
            }
            case Vectorgraphic::Type:
            {
                Vectorgraphic *v = dynamic_cast<Vectorgraphic *>(item);
                dataStream << Vectorgraphic::Type;
                dataStream << v->id();
                dataStream << v->pos().x();
                dataStream << v->pos().y();
                dataStream << v->xscale();
                dataStream << v->yscale();
                dataStream << v->opacity();
                dataStream << v->getData();

                writeKeyframes(dataStream, v);
                break;
            }
            default:
                break;
        }
    }
    return dataStream;
}

void AnimationScene::copyItem()
{
    if(selectedItems().count() == 0)
        return;

    QGraphicsItem *gi = selectedItems().first();
    m_copy = dynamic_cast<ResizeableItem*>(gi);
}

void AnimationScene::pasteItem()
{
    if(m_copy == NULL)
        return;

    m_copy->setSelected(false);
    switch(m_copy->type())
    {
        case Rectangle::Type:
        {
            Rectangle *r = new Rectangle(m_copy->rect().width(), m_copy->rect().height(), this);
            r->setPos(m_copy->pos().x() + 10, m_copy->pos().y() + 10);
            r->setId("");
            r->setPen(m_copy->pen());
            r->setBrush(m_copy->brush());
            r->setFlag(QGraphicsItem::ItemIsMovable, true);
            r->setFlag(QGraphicsItem::ItemIsSelectable, true);
            addItem(r);
            emit itemAdded(r);
            break;
        }
        case Ellipse::Type:
        {
            Ellipse *e = new Ellipse(m_copy->rect().width(), m_copy->rect().height(), this);
            e->setPos(m_copy->pos().x() + 10, m_copy->pos().y() + 10);
            e->setId("");
            e->setPen(m_copy->pen());
            e->setBrush(m_copy->brush());
            e->setFlag(QGraphicsItem::ItemIsMovable, true);
            e->setFlag(QGraphicsItem::ItemIsSelectable, true);
            addItem(e);
            emit itemAdded(e);
            break;
        }
        case Text::Type:
        {
            Text *cpy = dynamic_cast<Text*>(m_copy);
            Text *t = new Text(cpy->text(), this);
            t->setId("");
            t->setPos(m_copy->pos().x() + 10, m_copy->pos().y() + 10);
            t->setFlag(QGraphicsItem::ItemIsMovable, true);
            t->setFlag(QGraphicsItem::ItemIsSelectable, true);
            t->setScale(cpy->xscale(), cpy->yscale());
            t->setTextcolor(cpy->textcolor());
            addItem(t);
            emit itemAdded(t);
            break;
        }
        case Bitmap::Type:
        {
            Bitmap *bm = dynamic_cast<Bitmap*>(m_copy);
            Bitmap *b = new Bitmap(bm->getImage(), bm->rect().width(), bm->rect().height(), this);
            b->setId("");
            b->setPos(m_copy->pos().x() + 10, m_copy->pos().y() + 10);
            b->setFlag(QGraphicsItem::ItemIsMovable, true);
            b->setFlag(QGraphicsItem::ItemIsSelectable, true);
            b->setScale(bm->xscale(), bm->yscale());
            addItem(b);
            emit itemAdded(b);
            break;
        }
        case Vectorgraphic::Type:
        {
            Vectorgraphic *vg = dynamic_cast<Vectorgraphic*>(m_copy);
            Vectorgraphic *v = new Vectorgraphic(vg->getData(), this);
            v->setId("");
            v->setPos(m_copy->pos().x() + 10, m_copy->pos().y() + 10);
            v->setFlag(QGraphicsItem::ItemIsMovable, true);
            v->setFlag(QGraphicsItem::ItemIsSelectable, true);
            v->setScale(vg->xscale(), vg->yscale());
            addItem(v);
            emit itemAdded(v);
            break;
        }
    }
}

void AnimationScene::setPlayheadPosition(int val)
{
    m_playheadPosition = val;

    for(int i=0; i < items().count(); i++)
    {
        ResizeableItem *item = dynamic_cast<ResizeableItem *>(items().at(i));
        if(item)
        {
            QHash<QString, KeyFrame*>::iterator it;
            for (it = item->keyframes()->begin(); it != item->keyframes()->end(); ++it)
            {
                KeyFrame *found = NULL;
                KeyFrame *first = it.value();
                for(KeyFrame *frame = first; frame != NULL; frame = frame->next())
                {
                    if((frame == first && val < first->time()) || frame->time() <= val)
                        found = frame;
                }
                if(found)
                {
                    QString propertyName = it.key();
                    qreal value;
                    if(found->easing() >= 0)
                    {
                        QEasingCurve easing((QEasingCurve::Type)found->easing());
                        qreal progress = 1.0 / (found->next()->time() - found->time()) * (val - found->time());
                        qreal progressValue = easing.valueForProgress(progress);
                        value = found->value().toReal() + (found->next()->value().toReal() - found->value().toReal()) / 1.0 * progressValue;
                    }
                    else
                        value = found->value().toReal();
                    if(propertyName == "left")
                        item->setX(value);
                    else if(propertyName == "top")
                        item->setY(value);
                    else if(propertyName == "width")
                        item->setWidth(value);
                    else if(propertyName == "height")
                        item->setHeight(value);
                    else if(propertyName == "opacity")
                        item->setOpacity(value);
                }
            }
        }
    }
}

QDataStream& operator <<(QDataStream &out, const AnimationScene *s)
{
    return s->write(out);
}

QDataStream& operator >>(QDataStream &in, AnimationScene *s)
{
    return s->read(in);
}

QString getItemTypeName(ResizeableItem *item)
{
    switch(item->type())
    {
        case Rectangle::Type:
        {
            return QString("Rectangle");
        }
        case Ellipse::Type:
        {
            return QString("Ellipse");
        }
        case Text::Type:
        {
            return QString("Text");
        }
        case Bitmap::Type:
        {
            return QString("Bitmap");
        }
        case Vectorgraphic::Type:
        {
            return QString("Vectorgraphic");
        }
        default:
        qWarning() << "unknown item type: " << item->type();
        break;
    }
    return QString();
}

bool isAnimationMakerItem(QGraphicsItem *item)
{
    switch(item->type())
    {
        case Rectangle::Type:
        case Ellipse::Type:
        case Text::Type:
        case Bitmap::Type:
        case Vectorgraphic::Type:
            return true;
    }
    return false;
}
