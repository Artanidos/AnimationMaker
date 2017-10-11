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
#include "animationitem.h"
#include "plugins.h"
#include "ellipse.h"
#include "text.h"
#include "bitmap.h"
#include "vectorgraphic.h"
#include "keyframe.h"
#include "commands.h"
#include <QUndoCommand>
#include <QBuffer>
#include <QMessageBox>

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
    m_movingItem = NULL;
    addBackgroundRect();
}

void AnimationScene::reset()
{
    clear();
    initialize();
    m_undoStack->clear();
}

int AnimationScene::fps() const
{
    return m_fps;
}

void AnimationScene::deleteItem(AnimationItem *item)
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
        for(int i=0; i < itemList.count(); i++)
        {
            m_movingItem = dynamic_cast<AnimationItem*>(itemList.at(i));
            if(m_movingItem)
            {
                m_oldPos = m_movingItem->pos();
                break;
            }
        }
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

void AnimationScene::setEditMode(QString pluginName)
{
    m_editMode = EditMode::ModePlugin;
    m_actPluginName = pluginName;
}

void AnimationScene::addBackgroundRect()
{
    m_rect = new QGraphicsRectItem(0, 0, width(), height());
    m_backgroundColor = QColor("#404244");
    m_rect->setBrush(QBrush(QColor(m_backgroundColor)));
    m_rect->setPos(0,0);
    addItem(m_rect);
}

void AnimationScene::readKeyframes(QDataStream &dataStream, AnimationItem *item)
{
    int vars, easing, time, keyframes;
    QVariant value;
    QString propertyName;

    dataStream >> vars;

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
    QFont font;

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
            dataStream >> font;

            Text *t = new Text(text, this);
            t->setId(id);
            t->setPos(x, y);
            t->setFlag(QGraphicsItem::ItemIsMovable, true);
            t->setFlag(QGraphicsItem::ItemIsSelectable, true);
            t->setScale(xscale, yscale);
            t->setTextcolor(color);
            t->setOpacity(opacity);
            t->setFont(font);

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

            if(m_fileVersion > 100)
            {
                int attributes;
                QString name, value;

                dataStream >> attributes;
                for(int i = 0; i < attributes; i++)
                {
                    dataStream >> name;
                    dataStream >> value;
                    v->setAttributeValue(name, value);
                }
            }
            readKeyframes(dataStream, v);

            addItem(v);
        }
    }

    return dataStream;
}

bool AnimationScene::importXml(QString fileName)
{
    bool fullyLoaded = false;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0, "Error", "Unable to open file " + fileName);
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        QMessageBox::warning(0, "Error", "Unable to read file " + fileName);
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    if(docElem.nodeName() == "Animation")
    {
        emit animationResetted();
        setFps(docElem.attribute("fps", "24").toInt());
        setWidth(docElem.attribute("width", "1200").toInt());
        setHeight(docElem.attribute("height", "720").toInt());
        fullyLoaded = true;
    }
    for(int i=0; i < docElem.childNodes().count(); i++)
    {
        QDomNode node = docElem.childNodes().at(i);
        if(node.nodeName() == "Rectangle")
        {
            QDomElement ele = node.toElement();
            Rectangle *r = new Rectangle(ele.attribute("width", "50").toDouble(), ele.attribute("height", "50").toDouble(), this);
            r->setId(ele.attribute("id", "Rectangle"));
            r->setLeft(ele.attribute("left", "0").toDouble());
            r->setTop(ele.attribute("top", "0").toDouble());
            r->setPen(QPen(QColor(ele.attribute("pen", "#000000"))));
            r->setBrush(QBrush(QColor(ele.attribute("brush", "#0000FF"))));
            r->setOpacity(ele.attribute("opacity", "100").toInt());
            r->setFlag(QGraphicsItem::ItemIsMovable, true);
            r->setFlag(QGraphicsItem::ItemIsSelectable, true);
            readKeyframes(&ele, r);
            addItem(r);
        }
        else if(node.nodeName() == "Ellipse")
        {
            QDomElement ele = node.toElement();
            Ellipse *e = new Ellipse(ele.attribute("width", "50").toDouble(), ele.attribute("height", "50").toDouble(), this);
            e->setId(ele.attribute("id", "Ellipse"));
            e->setHeight(ele.attribute("height", "50").toDouble());
            e->setLeft(ele.attribute("left", "0").toDouble());
            e->setTop(ele.attribute("top", "0").toDouble());
            e->setPen(QPen(QColor(ele.attribute("pen", "#000000"))));
            e->setBrush(QBrush(QColor(ele.attribute("brush", "#0000FF"))));
            e->setOpacity(ele.attribute("opacity", "100").toInt());
            e->setFlag(QGraphicsItem::ItemIsMovable, true);
            e->setFlag(QGraphicsItem::ItemIsSelectable, true);
            readKeyframes(&ele, e);
            addItem(e);
        }
        else if(node.nodeName() == "Text")
        {
            QDomElement ele = node.toElement();
            Text *t = new Text(ele.attribute("text"), this);
            t->setId(ele.attribute("id", "Text"));
            t->setLeft(ele.attribute("left", "0").toDouble());
            t->setTop(ele.attribute("top", "0").toDouble());
            t->setScale(ele.attribute("xscale", "1").toDouble(), ele.attribute("yscale", "1").toDouble());
            t->setTextcolor(QColor(ele.attribute("textcolor", "#000000")));
            t->setOpacity(ele.attribute("opacity", "100").toInt());
            QFont font;
            font.setFamily(ele.attribute("font-family"));
            font.setPointSize(ele.attribute("font-size").toInt());
            font.setStyleName(ele.attribute("font-style"));
            t->setFont(font);
            t->setFlag(QGraphicsItem::ItemIsMovable, true);
            t->setFlag(QGraphicsItem::ItemIsSelectable, true);
            readKeyframes(&ele, t);
            addItem(t);
        }
        else if(node.nodeName() == "Bitmap")
        {
            QDomElement ele = node.toElement();
            QDomNode data = ele.firstChild();
            QDomCDATASection cdata = data.toCDATASection();
            QImage img = QImage::fromData(QByteArray::fromBase64(cdata.data().toLatin1()), "PNG");
            Bitmap *b = new Bitmap(img, ele.attribute("width", "50").toDouble(), ele.attribute("height", "50").toDouble(), this);
            b->setId(ele.attribute("id", "Bitmap"));
            b->setLeft(ele.attribute("left", "0").toDouble());
            b->setTop(ele.attribute("top", "0").toDouble());
            b->setOpacity(ele.attribute("opacity", "100").toInt());
            b->setFlag(QGraphicsItem::ItemIsMovable, true);
            b->setFlag(QGraphicsItem::ItemIsSelectable, true);
            readKeyframes(&ele, b);
            addItem(b);
        }
        else if(node.nodeName() == "Vectorgraphic")
        {
            QDomElement ele = node.toElement();
            QDomNode data = ele.firstChild();
            QDomCDATASection cdata = data.toCDATASection();
            Vectorgraphic *v = new Vectorgraphic(cdata.data().toLatin1(), this);
            v->setId(ele.attribute("id", "Vectorgraphic"));
            v->setLeft(ele.attribute("left", "0").toDouble());
            v->setTop(ele.attribute("top", "0").toDouble());
            v->setScale(ele.attribute("xscale", "1").toDouble(), ele.attribute("yscale", "1").toDouble());
            v->setOpacity(ele.attribute("opacity", "100").toInt());
            v->setFlag(QGraphicsItem::ItemIsMovable, true);
            v->setFlag(QGraphicsItem::ItemIsSelectable, true);
            for(int j = 0; j < ele.childNodes().count(); j++)
            {
                QDomNode nodej = ele.childNodes().at(j);
                if(nodej.nodeName() == "Attributes")
                {
                    QDomElement elej = nodej.toElement();
                    for(int k = 0; k < elej.childNodes().count(); k++)
                    {
                        QDomNode nodek = elej.childNodes().at(k);
                        if(nodek.nodeName() == "Attribute")
                        {
                            QDomElement elek = nodek.toElement();
                            v->setAttributeValue(elek.attribute("attributeName"), elek.attribute("value"));
                        }
                    }
                }
            }
            readKeyframes(&ele, v);
            addItem(v);
        }
        else
        {
            // found elememt from plugin
            foreach(QString key, Plugins::itemPluginNames())
            {
                ItemInterface *item = Plugins::getItemPlugin(key);
                if(node.nodeName() == item->tagName())
                {
                    QDomElement ele = node.toElement();
                    AnimationItem *i = item->getInstanceFromXml(this, ele);
                    i->setFlag(QGraphicsItem::ItemIsMovable, true);
                    i->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    readKeyframes(&ele, i);
                    addItem(i);
                }
            }
        }
    }
    return fullyLoaded;
}

void AnimationScene::exportXml(QString fileName, bool exportAll)
{   
    QDomDocument doc;
    QDomElement root;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(0, "Error", "Unable to open file " + fileName);
        return;
    }

    if(exportAll)
    {
        root = doc.createElement("Animation");
        root.setAttribute("fps", fps());
        root.setAttribute("width", width());
        root.setAttribute("height", height());
        doc.appendChild(root);
    }
    else
    {
        root = doc.createElement("AnimationItems");
        doc.appendChild(root);
    }

    for(int i=0; i < items().count(); i++)
    {
        AnimationItem *item = dynamic_cast<AnimationItem*>(items().at(i));
        if(item)
        {
            QDomElement ele = item->getXml(doc);
            writeKeyframes(&doc, &ele, item);
            root.appendChild(ele);
        }
    }
    QTextStream stream(&file);
    stream << doc.toString();
    file.close();
}

void AnimationScene::writeKeyframes(QDomDocument *doc, QDomElement *element, AnimationItem *item)
{
    QHash<QString, KeyFrame*>::iterator it;
    for(it = item->keyframes()->begin(); it != item->keyframes()->end(); it++)
    {
        QDomElement frames = doc->createElement("Keyframes");
        frames.setAttribute("property", it.key());
        for(KeyFrame *frame = it.value(); frame != NULL; frame = frame->next())
        {
            QDomElement f = doc->createElement("Keyframe");
            f.setAttribute("time", frame->time());
            f.setAttribute("value", frame->value().toString());
            f.setAttribute("easing", frame->easing());
            f.setAttribute("type", frame->value().typeName());
            frames.appendChild(f);
        }
        element->appendChild(frames);
    }
}

void AnimationScene::readKeyframes(QDomElement *element, AnimationItem *item)
{
    KeyFrame *m_tempKeyFrame = NULL;
    for(int i=0; i < element->childNodes().count(); i++)
    {
        QDomNode node = element->childNodes().at(i);
        if(node.nodeName() == "Keyframes")
        {
            QDomElement keyframes = node.toElement();
            QString property = keyframes.attribute("property");
            for(int j=0; j < node.childNodes().count(); j++)
            {
                QDomNode frameNode = node.childNodes().at(j);
                if(frameNode.nodeName() == "Keyframe")
                {
                    QString defaultType = QVariant::typeToName(QVariant::Type::Double);
                    // in version 1.5 we introduced svg attribute animation without declaring a type
                    // all these properties have got a dot in its name like "element.attribute"
                    // all of these attributes where handled as strings because we don't know there real type
                    // also the fill attribute in svg for example can contain a color string like "#00ffbb" or "none"
                    if(property.contains("."))
                        defaultType = QVariant::typeToName(QVariant::Type::String);
                    QDomElement keyframe = frameNode.toElement();
                    KeyFrame *key = new KeyFrame();
                    QString type = keyframe.attribute("type", defaultType);
                    QString value = keyframe.attribute("value");
                    key->setTime(keyframe.attribute("time", "0").toInt());
                    if(type == QVariant::typeToName(QVariant::Type::Double))
                    {
                        key->setValue(QVariant(value.toDouble()));
                    }
                    else if(type == QVariant::typeToName(QVariant::Type::Int))
                    {
                        key->setValue(QVariant(value.toInt()));
                    }
                    else if(type == QVariant::typeToName(QVariant::Type::Color))
                    {
                        key->setValue(QVariant(QColor(value)));
                    }
                    else if(type == QVariant::typeToName(QVariant::Type::String))
                    {
                        key->setValue(QVariant(value));
                    }
                    else
                    {
                        qDebug() << "keyframe type not implemented" << type;
                        key->setValue(value);
                    }
                    key->setEasing(keyframe.attribute("easing", "-1").toInt());
                    // set double linked list
                    if(m_tempKeyFrame)
                    {
                        m_tempKeyFrame->setNext(key);
                        key->setPrev(m_tempKeyFrame);
                    }
                    else
                    {
                        item->addKeyframe(property, key);
                        emit keyframeAdded(item, property, key);
                    }
                    m_tempKeyFrame = key;
                }
            }
            m_tempKeyFrame = NULL;
        }
    }
}

void AnimationScene::copyItem()
{
    if(selectedItems().count() == 0)
        return;

    QGraphicsItem *gi = selectedItems().first();
    m_copy = dynamic_cast<AnimationItem*>(gi);
}

void AnimationScene::copyKeyframes(AnimationItem *item)
{
    QHash<QString, KeyFrame*>::iterator it;
    for (it = m_copy->keyframes()->begin(); it != m_copy->keyframes()->end(); ++it)
    {
        KeyFrame *first = it.value();
        KeyFrame *firstFrame = new KeyFrame();
        firstFrame->setEasing(first->easing());
        firstFrame->setTime(first->time());
        firstFrame->setValue(first->value());
        firstFrame->setNext(NULL);
        firstFrame->setPrev(NULL);
        item->addKeyframe(it.key(), firstFrame);
        KeyFrame *last = firstFrame;
        for(KeyFrame *frame = first->next(); frame != NULL; frame = frame->next())
        {
            KeyFrame *nextFrame = new KeyFrame();
            nextFrame->setEasing(frame->easing());
            nextFrame->setTime(frame->time());
            nextFrame->setValue(frame->value());
            nextFrame->setNext(NULL);
            nextFrame->setPrev(last);
            last->setNext(nextFrame);
            last = nextFrame;
        }
        emit keyframeAdded(item, it.key(), firstFrame);
    }
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
            r->setId("Rectangle");
            r->setPen(m_copy->pen());
            r->setBrush(m_copy->brush());
            r->setFlag(QGraphicsItem::ItemIsMovable, true);
            r->setFlag(QGraphicsItem::ItemIsSelectable, true);
            copyKeyframes(r);
            addItem(r);
            emit itemAdded(r);
            break;
        }
        case Ellipse::Type:
        {
            Ellipse *e = new Ellipse(m_copy->rect().width(), m_copy->rect().height(), this);
            e->setPos(m_copy->pos().x() + 10, m_copy->pos().y() + 10);
            e->setId("Ellipse");
            e->setPen(m_copy->pen());
            e->setBrush(m_copy->brush());
            e->setFlag(QGraphicsItem::ItemIsMovable, true);
            e->setFlag(QGraphicsItem::ItemIsSelectable, true);
            copyKeyframes(e);
            addItem(e);
            emit itemAdded(e);
            break;
        }
        case Text::Type:
        {
            Text *cpy = dynamic_cast<Text*>(m_copy);
            Text *t = new Text(cpy->text(), this);
            t->setId("Text");
            t->setPos(m_copy->pos().x() + 10, m_copy->pos().y() + 10);
            t->setFlag(QGraphicsItem::ItemIsMovable, true);
            t->setFlag(QGraphicsItem::ItemIsSelectable, true);
            t->setTextcolor(cpy->textcolor());
            t->setFont(cpy->font());
            t->setScale(cpy->xscale(), cpy->yscale());
            copyKeyframes(t);
            addItem(t);
            emit itemAdded(t);
            break;
        }
        case Bitmap::Type:
        {
            Bitmap *bm = dynamic_cast<Bitmap*>(m_copy);
            Bitmap *b = new Bitmap(bm->getImage(), bm->rect().width(), bm->rect().height(), this);
            b->setId("Bitmap");
            b->setPos(m_copy->pos().x() + 10, m_copy->pos().y() + 10);
            b->setFlag(QGraphicsItem::ItemIsMovable, true);
            b->setFlag(QGraphicsItem::ItemIsSelectable, true);
            b->setScale(bm->xscale(), bm->yscale());
            copyKeyframes(b);
            addItem(b);
            emit itemAdded(b);
            break;
        }
        case Vectorgraphic::Type:
        {
            Vectorgraphic *vg = dynamic_cast<Vectorgraphic*>(m_copy);
            Vectorgraphic *v = new Vectorgraphic(vg->getData(), this);
            v->setId("Vectorgraphic");
            v->setPos(m_copy->pos().x() + 10, m_copy->pos().y() + 10);
            v->setFlag(QGraphicsItem::ItemIsMovable, true);
            v->setFlag(QGraphicsItem::ItemIsSelectable, true);
            v->setScale(vg->xscale(), vg->yscale());
            copyKeyframes(v);
            addItem(v);
            emit itemAdded(v);
            break;
        }
    }
}

void AnimationScene::setPlayheadPosition(int playheadPosition)
{
    m_playheadPosition = playheadPosition;

    for(int i=0; i < items().count(); i++)
    {
        AnimationItem *item = dynamic_cast<AnimationItem *>(items().at(i));
        if(item)
        {
            item->setPlayheadPosition(m_playheadPosition);
            QHash<QString, KeyFrame*>::iterator it;
            for (it = item->keyframes()->begin(); it != item->keyframes()->end(); ++it)
            {
                KeyFrame *found = NULL;
                KeyFrame *first = it.value();
                for(KeyFrame *frame = first; frame != NULL; frame = frame->next())
                {
                    if((frame == first && m_playheadPosition < first->time()) || frame->time() <= m_playheadPosition)
                        found = frame;
                }
                if(found)
                {
                    QString propertyName = it.key();
                    int dot = propertyName.indexOf(".");
                    if(dot > 0)
                    {
                        Vectorgraphic *vec = dynamic_cast<Vectorgraphic *>(items().at(i));
                        if(vec)
                        {
                            QString value;
                            if(found->easing() >= 0)
                            {
                                qreal progressValue = getProgressValue(found, playheadPosition);
                                QString foundValue = found->value().toString();
                                bool isInteger;
                                int intVal = foundValue.toInt(&isInteger);
                                if(isInteger)
                                {
                                    int nextVal = found->next()->value().toString().toInt();
                                    value = QString::number(intVal + (nextVal - intVal) / 1.0 * progressValue);
                                }
                                else
                                {
                                    bool isColor;
                                    foundValue.mid(1).toInt(&isColor, 16);
                                    if(foundValue.startsWith("#") && isColor)
                                        value = calculateColorValue(found, playheadPosition).name();
                                    else
                                        value = found->value().toString();
                                }
                            }
                            else
                                value = found->value().toString();
                            vec->setAttributeValue(propertyName, value);
                        }
                    }
                    else
                    {
                        switch(found->value().type())
                        {
                            case QVariant::Type::Double:
                            {
                                qreal value;
                                if(found->easing() >= 0)
                                    value = calculateRealValue(found, m_playheadPosition);
                                else
                                    value = found->value().toReal();
                                item->setProperty(propertyName.toLatin1(), value);
                                break;
                            }
                            case QVariant::Type::Int:
                            {
                                int value;
                                if(found->easing() >= 0)
                                    value = calculateIntValue(found, m_playheadPosition);
                                else
                                    value = found->value().toInt();
                                item->setProperty(propertyName.toLatin1(), value);
                                break;
                            }
                            case QVariant::Type::Color:
                            {
                                QColor value;
                                if(found->easing() >= 0)
                                    value = calculateColorValue(found, m_playheadPosition);
                                else
                                    value = found->value().value<QColor>();
                                item->setProperty(propertyName.toLatin1(), value);
                                break;
                            }
                            default:
                                qDebug() << propertyName << found->value().type() << "not implemented yet";
                                break;
                        }

                    }
                }
            }
        }
    }
}

qreal getProgressValue(KeyFrame *found, int playheadPosition)
{
    QEasingCurve easing((QEasingCurve::Type)found->easing());
    qreal progress = 1.0 / (found->next()->time() - found->time()) * (playheadPosition - found->time());
    return easing.valueForProgress(progress);
}

qreal calculateRealValue(KeyFrame *found, int playheadPosition)
{
    return found->value().toReal() + (found->next()->value().toReal() - found->value().toReal()) / 1.0 * getProgressValue(found, playheadPosition);
}

int calculateIntValue(KeyFrame *found, int playheadPosition)
{
    return (int)found->value().toInt() + (found->next()->value().toInt() - found->value().toInt()) / 1.0 * getProgressValue(found, playheadPosition);
}

QColor calculateColorValue(KeyFrame *found, int playheadPosition)
{
    qreal progressValue = getProgressValue(found, playheadPosition);

    QColor fromColor = found->value().value<QColor>();
    QColor toColor = found->next()->value().value<QColor>();
    //QColor fromColor(found->value().toString());
    //QColor toColor(found->next()->value().toString());
    int fromRed = fromColor.red();
    int fromBlue = fromColor.blue();
    int fromGreen = fromColor.green();

    int toRed = toColor.red();
    int toBlue = toColor.blue();
    int toGreen = toColor.green();

    int newRed = fromRed + (toRed - fromRed) / 1.0 * progressValue;
    int newBlue = fromBlue + (toBlue - fromBlue) / 1.0 * progressValue;
    int newGreen = fromGreen + (toGreen - fromGreen) / 1.0 * progressValue;

    return QColor(newRed, newGreen, newBlue);
}

QDataStream& operator >>(QDataStream &in, AnimationScene *s)
{
    return s->read(in);
}
