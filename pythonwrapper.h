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

#ifndef PYTHONWRAPPER_H
#define PYTHONWRAPPER_H

#include <QVariant>
#include <QBuffer>
#include "animationscene.h"
#include "rectangle.h"
#include "ellipse.h"
#include "text.h"
#include "bitmap.h"
#include "vectorgraphic.h"
#include "resizeableitem.h"

class KeyFrameWrapper : public QObject
{
    Q_OBJECT
public Q_SLOTS:
    KeyFrame* new_KeyFrame() { return new KeyFrame();}
    int time(KeyFrame *o) {return o->time();}
    void setTime(KeyFrame *o, int value) {o->setTime(value);}
    int easing(KeyFrame *o) {return o->easing();}
    void setEasing(KeyFrame *o, int value) {o->setEasing(value);}
    QVariant value(KeyFrame *o) {return o->value();}
    void setValue(KeyFrame *o, QVariant value) {o->setValue(value);}
    QVariant next(KeyFrame *o) {return qVariantFromValue<KeyFrame*>(o->next());}
    void setNext(KeyFrame *o, KeyFrame *value) {o->setNext(value);}
    void setPrev(KeyFrame *o, KeyFrame *value) {o->setPrev(value);}
};

class ResizeableItemWrapper : public QObject
{
    Q_OBJECT
public Q_SLOTS:
    QString id(ResizeableItem *o) {return o->id();}
    void setId(ResizeableItem *o, QString value) {o->setId(value);}
    qreal left(ResizeableItem *o) {return o->left();}
    void setLeft(ResizeableItem *o, qreal value) {o->setLeft(value);}
    qreal top(ResizeableItem *o) {return o->top();}
    void setTop(ResizeableItem *o, qreal value) {o->setTop(value);}
    int opacity(ResizeableItem *o) {return o->opacity();}
    void setOpacity(ResizeableItem *o, int value) {o->setOpacity(value);}
    QMap<QString, QVariant> keyframes(ResizeableItem *o)
    {
        QMap<QString, QVariant> keyframes;
        QHash<QString, KeyFrame*>::iterator it;
        for(it = o->keyframes()->begin(); it != o->keyframes()->end(); it++)
        {
            keyframes.insert(it.key(), qVariantFromValue<KeyFrame*>(it.value()));
        }
        return keyframes;
    }
    void insertKeyframe(ResizeableItem *o, QString property, KeyFrame *key)
    {
        o->addKeyframe(property, key);
        emit o->scene()->keyframeAdded(o, property, key);
    }
};

class RectangleWrapper : public ResizeableItemWrapper
{
    Q_OBJECT
public Q_SLOTS:
    Rectangle* new_Rectangle() { return new Rectangle();}
    qreal width(Rectangle *o) {return o->rect().width();}
    void setWidth(Rectangle *o, qreal value) {o->setWidth(value);}
    qreal height(Rectangle *o) {return o->rect().height();}
    void setHeight(Rectangle *o, qreal value) {o->setHeight(value);}
    QString pen(Rectangle *o) {return o->pen().color().name();}
    void setPen(Rectangle *o, QString value) {o->setPen(QPen(QColor(value)));}
    QString brush(Rectangle *o) {return o->brush().color().name();}
    void setBrush(Rectangle *o, QString value) {o->setBrush(QBrush(QColor(value)));}
};

class EllipseWrapper : public ResizeableItemWrapper
{
    Q_OBJECT
public Q_SLOTS:
    Ellipse* new_Ellipse() { return new Ellipse();}
    qreal width(Ellipse *o) {return o->rect().width();}
    void setWidth(Ellipse *o, qreal value) {o->setWidth(value);}
    qreal height(Ellipse *o) {return o->rect().height();}
    void setHeight(Ellipse *o, qreal value) {o->setHeight(value);}
    QString pen(Ellipse *o) {return o->pen().color().name();}
    void setPen(Ellipse *o, QString value) {o->setPen(QPen(QColor(value)));}
    QString brush(Ellipse *o) {return o->brush().color().name();}
    void setBrush(Ellipse *o, QString value) {o->setBrush(QBrush(QColor(value)));}
};

class TextWrapper : public ResizeableItemWrapper
{
    Q_OBJECT
public Q_SLOTS:
    Text* new_Text() { return new Text("", NULL);}
    qreal xscale(Text *o) {return o->xscale();}
    void setXscale(Text *o, qreal value) {o->setScale(value, o->yscale());}
    qreal yscale(Text *o) {return o->yscale();}
    void setYscale(Text *o, qreal value) {o->setScale(o->xscale(), value);}
    QString text(Text *o) {return o->text();}
    void setText(Text *o, QString value) {o->setText(value);}
    QString textcolor(Text *o) {return o->textcolor().name();}
    void setTextcolor(Text *o, QString value) {o->setTextcolor(value);}
    QString fontFamily(Text *o) {return o->font().family();}
    void setFontFamily(Text *o, QString value)
    {
        QFont font;
        font.setFamily(value);
        font.setPointSize(o->font().pointSize());
        font.setStyleName(o->font().styleName());
        o->setFont(font);
    }
    int fontSize(Text *o) {return o->font().pointSize();}
    void setFontSize(Text *o, int value)
    {
        QFont font;
        font.setFamily(o->font().family());
        font.setPointSize(value);
        font.setStyleName(o->font().styleName());
        o->setFont(font);
    }
    QString fontStyle(Text *o) {return o->font().styleName();}
    void setFontStyle(Text *o, QString value)
    {
        QFont font;
        font.setFamily(o->font().family());
        font.setPointSize(o->font().pointSize());
        font.setStyleName(value);
        o->setFont(font);
    }
};

class BitmapWrapper : public ResizeableItemWrapper
{
    Q_OBJECT
public Q_SLOTS:
    Bitmap* new_Bitmap() { return new Bitmap();}
    qreal width(Bitmap *o) {return o->rect().width();}
    void setWidth(Bitmap *o, qreal value) {o->setWidth(value);}
    qreal height(Bitmap *o) {return o->rect().height();}
    void setHeight(Bitmap *o, qreal value) {o->setHeight(value);}
    QString image(Bitmap *o)
    {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        o->getImage().save(&buffer, "PNG");
        return QString::fromLatin1(byteArray.toBase64().data());
    }
    void setImage(Bitmap *o, QString value)
    {
        QImage img = QImage::fromData(QByteArray::fromBase64(value.toLatin1()), "PNG");
        o->setImage(img);
    }
};

class VectorgraphicWrapper : public ResizeableItemWrapper
{
    Q_OBJECT
public Q_SLOTS:
    Vectorgraphic* new_Vectorgraphic() { return new Vectorgraphic();}
    qreal xscale(Vectorgraphic *o) {return o->xscale();}
    void setXscale(Vectorgraphic *o, qreal value) {o->setScale(value, o->yscale());}
    qreal yscale(Vectorgraphic *o) {return o->yscale();}
    void setYscale(Vectorgraphic *o, qreal value) {o->setScale(o->xscale(), value);}
    QString data(Vectorgraphic *o) {return QString::fromLatin1(o->getData());}
    void setData(Vectorgraphic *o, QString value) {o->setData(value.toLatin1());}
};

class AnimationSceneWrapper : public QObject
{
    Q_OBJECT
public Q_SLOTS:
    void reset(AnimationScene *o) {emit o->animationResetted();}
    int fps(AnimationScene *o) { return o->fps(); }
    void setFps(AnimationScene *o, int value) {o->setFps(value);}
    int width(AnimationScene *o) { return o->width(); }
    void setWidth(AnimationScene *o, int value) {o->setWidth(value);}
    int height(AnimationScene *o) { return o->height(); }
    void setHeight(AnimationScene *o, int value) {o->setHeight(value);}
    void addItem(AnimationScene *o, Rectangle *item)
    {
        item->setFlag(QGraphicsItem::ItemIsMovable, true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item->setScene(o);
        o->addItem(item);
    }
    void addItem(AnimationScene *o, Ellipse *item)
    {
        item->setFlag(QGraphicsItem::ItemIsMovable, true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item->setScene(o);
        o->addItem(item);
    }
    void addItem(AnimationScene *o, Text *item)
    {
        item->setFlag(QGraphicsItem::ItemIsMovable, true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item->setScene(o);
        o->addItem(item);
    }
    void addItem(AnimationScene *o, Bitmap *item)
    {
        item->setFlag(QGraphicsItem::ItemIsMovable, true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item->setScene(o);
        o->addItem(item);
    }
    void addItem(AnimationScene *o, Vectorgraphic *item)
    {
        item->setFlag(QGraphicsItem::ItemIsMovable, true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item->setScene(o);
        o->addItem(item);
    }
    QVariantList items(AnimationScene *o)
    {
        QVariantList items;
        QList<QGraphicsItem*> itemList = o->items(Qt::AscendingOrder);
        foreach (QGraphicsItem *item, itemList)
        {
            Rectangle *r = dynamic_cast<Rectangle *>(item);
            if(r)
                items.append(qVariantFromValue<Rectangle*>(r));
            Ellipse *e = dynamic_cast<Ellipse *>(item);
            if(e)
                items.append(qVariantFromValue<Ellipse*>(e));
            Text *t = dynamic_cast<Text *>(item);
            if(t)
                items.append(qVariantFromValue<Text*>(t));
            Bitmap *b = dynamic_cast<Bitmap*>(item);
            if(b)
                items.append(qVariantFromValue<Bitmap*>(b));
            Vectorgraphic *v = dynamic_cast<Vectorgraphic*>(item);
            if(v)
                items.append(qVariantFromValue<Vectorgraphic*>(v));
        }
        return items;
    }
};

#endif // PYTHONWRAPPER_H
