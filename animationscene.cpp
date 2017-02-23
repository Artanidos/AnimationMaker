#include "animationscene.h"
#include "serializeableitem.h"
#include "handleitem.h"
#include "rectangle.h"
#include "ellipse.h"
#include "text.h"
#include "bitmap.h"
#include "vectorgraphic.h"

AnimationScene::AnimationScene()
{
    m_editMode = EditMode::ModeSelect;
    m_fps = 24;
    m_copy = NULL;
}

void AnimationScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    if(m_editMode == EditMode::ModeSelect)
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
    else if(m_editMode == EditMode::ModeRectangle)
    {
        clearSelection();

        Rectangle *r = new Rectangle(50, 50);
        r->setId("");
        r->setPen(QPen(Qt::black));
        r->setBrush(QBrush(Qt::blue));
        r->setFlag(QGraphicsItem::ItemIsMovable, true);
        r->setFlag(QGraphicsItem::ItemIsSelectable, true);
        r->setPos(mouseEvent->scenePos());
        connect(r, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimationRequested(ResizeableItem *, const QString, qreal, int, int)));
        addItem(r);
        emit itemAdded(r);
    }
    else if(m_editMode == EditMode::ModeEllipse)
    {
        clearSelection();

        Ellipse *e = new Ellipse(50, 50);
        e->setPen(QPen(Qt::black));
        e->setBrush(QBrush(Qt::blue));
        e->setFlag(QGraphicsItem::ItemIsMovable, true);
        e->setFlag(QGraphicsItem::ItemIsSelectable, true);
        e->setPos(mouseEvent->scenePos());
        connect(e, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimationRequested(ResizeableItem *, const QString, qreal, int, int)));
        addItem(e);
        emit itemAdded(e);
    }
    else if(m_editMode == EditMode::ModeText)
    {
        clearSelection();

        Text *t = new Text("Lorem ipsum dolor");
        t->setFlag(QGraphicsItem::ItemIsMovable, true);
        t->setFlag(QGraphicsItem::ItemIsSelectable, true);
        t->setPos(mouseEvent->scenePos());
        connect(t, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimationRequested(ResizeableItem *, const QString, qreal, int, int)));
        addItem(t);
        emit itemAdded(t);
    }
    else if(m_editMode == EditMode::ModeBitmap)
    {
        clearSelection();

        QString fileName = QFileDialog::getOpenFileName(0, tr("Open Bitmap"), "", tr("Image Files (*.png *.jpeg *.jpg *.gif *.bmp);;All Files (*)"));
        if (fileName.isEmpty())
            return;

        Bitmap *b = new Bitmap(fileName);
        b->setFlag(QGraphicsItem::ItemIsMovable, true);
        b->setFlag(QGraphicsItem::ItemIsSelectable, true);
        b->setPos(mouseEvent->scenePos());
        connect(b, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimationRequested(ResizeableItem *, const QString, qreal, int, int)));
        addItem(b);
        emit itemAdded(b);
    }
    else if(m_editMode == EditMode::ModeSvg)
    {
        clearSelection();

        QString fileName = QFileDialog::getOpenFileName(0, tr("Open SVG"), "", tr("SVG Files (*.svg);;All Files (*)"));
        if (fileName.isEmpty())
            return;

        Vectorgraphic *v = new Vectorgraphic(fileName);
        v->setFlag(QGraphicsItem::ItemIsMovable, true);
        v->setFlag(QGraphicsItem::ItemIsSelectable, true);
        v->setPos(mouseEvent->scenePos());
        connect(v, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimationRequested(ResizeableItem *, const QString, qreal, int, int)));
        addItem(v);
        emit itemAdded(v);
    }
}

void AnimationScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void AnimationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void AnimationScene::setEditMode(EditMode mode)
{
    m_editMode = mode;
}

QDataStream& AnimationScene::read(QDataStream &dataStream)
{
    int type, fps, animations, begin, duration, min, max;
    qreal x, y, z, width, height, xscale, yscale, start, end;
    QPen pen;
    QBrush brush;
    QString text, propertyName, id;
    QColor color;

    clear();
    dataStream >> width;
    dataStream >> height;
    dataStream >> fps;

    this->setSceneRect(0, 0, width, height);
    this->setFps(fps);

    while (!dataStream.atEnd())
    {
        dataStream >> type;
        if(type == Rectangle::Type)
        {
            dataStream >> id;
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> width;
            dataStream >> height;
            dataStream >> pen;
            dataStream >> brush;

            Rectangle *r = new Rectangle(width, height);
            r->setId(id);
            r->setPos(x, y);
            r->setPen(pen);
            r->setBrush(brush);
            r->setFlag(QGraphicsItem::ItemIsMovable, true);
            r->setFlag(QGraphicsItem::ItemIsSelectable, true);
            r->setZValue(z);

            dataStream >> animations;
            for(int i=0; i < animations; i++)
            {
                dataStream >> begin;
                dataStream >> min;
                dataStream >> max;
                dataStream >> duration;
                dataStream >> propertyName;
                dataStream >> start;
                dataStream >> end;
                QVariant b(begin);
                QVariant mn(min);
                QVariant mx(max);
                QPropertyAnimation *anim = new QPropertyAnimation();
                anim->setProperty("begin", b);
                anim->setProperty("min", mn);
                anim->setProperty("max", mx);
                anim->setTargetObject(r);
                anim->setPropertyName(propertyName.toLatin1());
                anim->setDuration(duration);
                anim->setStartValue(start);
                anim->setEndValue(end);
                r->addAnimation(anim);
                emit animationAdded(r, anim);
            }

            connect(r, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimationRequested(ResizeableItem *, const QString, qreal, int, int)));
            addItem(r);
        }
        else if(type == Ellipse::Type)
        {
            dataStream >> id;
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> width;
            dataStream >> height;
            dataStream >> pen;
            dataStream >> brush;

            Ellipse *e = new Ellipse(width, height);
            e->setId(id);
            e->setPos(x, y);
            e->setPen(pen);
            e->setBrush(brush);
            e->setFlag(QGraphicsItem::ItemIsMovable, true);
            e->setFlag(QGraphicsItem::ItemIsSelectable, true);
            e->setZValue(z);

            dataStream >> animations;
            for(int i=0; i < animations; i++)
            {
                dataStream >> begin;
                dataStream >> min;
                dataStream >> max;
                dataStream >> duration;
                dataStream >> propertyName;
                dataStream >> start;
                dataStream >> end;
                QVariant b(begin);
                QVariant mn(min);
                QVariant mx(max);
                QPropertyAnimation *anim = new QPropertyAnimation();
                anim->setProperty("begin", b);
                anim->setProperty("min", mn);
                anim->setProperty("max", mx);
                anim->setTargetObject(e);
                anim->setPropertyName(propertyName.toLatin1());
                anim->setDuration(duration);
                anim->setStartValue(start);
                anim->setEndValue(end);
                e->addAnimation(anim);
                emit animationAdded(e, anim);
            }
            connect(e, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimationRequested(ResizeableItem *, const QString, qreal, int, int)));
            addItem(e);
        }
        else if(type == Text::Type)
        {
            dataStream >> id;
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> xscale;
            dataStream >> yscale;
            dataStream >> text;
            dataStream >> color;

            Text *t = new Text(text);
            t->setId(id);
            t->setPos(x, y);
            t->setFlag(QGraphicsItem::ItemIsMovable, true);
            t->setFlag(QGraphicsItem::ItemIsSelectable, true);
            t->setZValue(z);
            t->setScale(xscale, yscale);
            t->setTextcolor(color);

            dataStream >> animations;
            for(int i=0; i < animations; i++)
            {
                dataStream >> begin;
                dataStream >> min;
                dataStream >> max;
                dataStream >> duration;
                dataStream >> propertyName;
                dataStream >> start;
                dataStream >> end;
                QVariant b(begin);
                QVariant mn(min);
                QVariant mx(max);
                QPropertyAnimation *anim = new QPropertyAnimation();
                anim->setProperty("begin", b);
                anim->setProperty("min", mn);
                anim->setProperty("max", mx);
                anim->setTargetObject(t);
                anim->setPropertyName(propertyName.toLatin1());
                anim->setDuration(duration);
                anim->setStartValue(start);
                anim->setEndValue(end);
                t->addAnimation(anim);
                emit animationAdded(t, anim);
            }
            connect(t, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimationRequested(ResizeableItem *, const QString, qreal, int, int)));
            addItem(t);
        }
        else if(type == Bitmap::Type)
        {
            QImage img;
            dataStream >> id;
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> width;
            dataStream >> height;
            dataStream >> img;

            Bitmap *b = new Bitmap(img, width, height);
            b->setId(id);
            b->setPos(x, y);
            b->setFlag(QGraphicsItem::ItemIsMovable, true);
            b->setFlag(QGraphicsItem::ItemIsSelectable, true);
            b->setZValue(z);
            b->setScale(xscale, yscale);

            dataStream >> animations;
            for(int i=0; i < animations; i++)
            {
                dataStream >> begin;
                dataStream >> min;
                dataStream >> max;
                dataStream >> duration;
                dataStream >> propertyName;
                dataStream >> start;
                dataStream >> end;
                QVariant beg(begin);
                QVariant mn(min);
                QVariant mx(max);
                QPropertyAnimation *anim = new QPropertyAnimation();
                anim->setProperty("begin", beg);
                anim->setProperty("min", mn);
                anim->setProperty("max", mx);
                anim->setTargetObject(b);
                anim->setPropertyName(propertyName.toLatin1());
                anim->setDuration(duration);
                anim->setStartValue(start);
                anim->setEndValue(end);
                b->addAnimation(anim);
                emit animationAdded(b, anim);
            }
            connect(b, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimationRequested(ResizeableItem *, const QString, qreal, int, int)));
            addItem(b);
        }
        else if(type == Vectorgraphic::Type)
        {
            QByteArray arr;
            dataStream >> id;
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> xscale;
            dataStream >> yscale;
            dataStream >> arr;
            Vectorgraphic *v = new Vectorgraphic(arr);
            v->setId(id);
            v->setPos(x, y);
            v->setFlag(QGraphicsItem::ItemIsMovable, true);
            v->setFlag(QGraphicsItem::ItemIsSelectable, true);
            v->setZValue(z);
            v->setScale(xscale, yscale);

            dataStream >> animations;
            for(int i=0; i < animations; i++)
            {
                dataStream >> begin;
                dataStream >> min;
                dataStream >> max;
                dataStream >> duration;
                dataStream >> propertyName;
                dataStream >> start;
                dataStream >> end;
                QVariant b(begin);
                QVariant mn(min);
                QVariant mx(max);
                QPropertyAnimation *anim = new QPropertyAnimation();
                anim->setProperty("begin", b);
                anim->setProperty("min", mn);
                anim->setProperty("max", mx);
                anim->setTargetObject(v);
                anim->setPropertyName(propertyName.toLatin1());
                anim->setDuration(duration);
                anim->setStartValue(start);
                anim->setEndValue(end);
                v->addAnimation(anim);
                emit animationAdded(v, anim);
            }
            connect(v, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimationRequested(ResizeableItem *, const QString, qreal, int, int)));
            addItem(v);
        }
    }

    return dataStream;
}

QDataStream& AnimationScene::write(QDataStream &dataStream) const
{
    dataStream << width();
    dataStream << height();
    dataStream << fps();

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
                dataStream << r->zValue();
                dataStream << r->rect().width();
                dataStream << r->rect().height();
                dataStream << r->pen();
                dataStream << r->brush();
                dataStream << r->getAnimationCount();
                for(int i=0; i< r->getAnimationCount(); i++)
                {
                    QPropertyAnimation *anim = r->getAnimation(i);
                    dataStream << anim->property("begin").toInt();
                    dataStream << anim->property("min").toInt();
                    dataStream << anim->property("max").toInt();
                    dataStream << anim->duration();
                    dataStream << QString(anim->propertyName());
                    dataStream << anim->startValue().toReal();
                    dataStream << anim->endValue().toReal();
                }
                break;
            }
            case Ellipse::Type:
            {
                Ellipse *e = dynamic_cast<Ellipse *>(item);
                dataStream << Ellipse::Type;
                dataStream << e->id();
                dataStream << e->pos().x();
                dataStream << e->pos().y();
                dataStream << e->zValue();
                dataStream << e->rect().width();
                dataStream << e->rect().height();
                dataStream << e->pen();
                dataStream << e->brush();
                dataStream << e->getAnimationCount();
                for(int i=0; i< e->getAnimationCount(); i++)
                {
                    QPropertyAnimation *anim = e->getAnimation(i);
                    dataStream << anim->property("begin").toInt();
                    dataStream << anim->property("min").toInt();
                    dataStream << anim->property("max").toInt();
                    dataStream << anim->duration();
                    dataStream << QString(anim->propertyName());
                    dataStream << anim->startValue().toReal();
                    dataStream << anim->endValue().toReal();
                }
                break;
            }
            case Text::Type:
            {
                Text *t = dynamic_cast<Text *>(item);
                dataStream << Text::Type;
                dataStream << t->id();
                dataStream << t->pos().x();
                dataStream << t->pos().y();
                dataStream << t->zValue();
                dataStream << t->xscale();
                dataStream << t->yscale();
                dataStream << t->text();
                dataStream << t->textcolor();
                dataStream << t->getAnimationCount();
                for(int i=0; i< t->getAnimationCount(); i++)
                {
                    QPropertyAnimation *anim = t->getAnimation(i);
                    dataStream << anim->property("begin").toInt();
                    dataStream << anim->property("min").toInt();
                    dataStream << anim->property("max").toInt();
                    dataStream << anim->duration();
                    dataStream << QString(anim->propertyName());
                    dataStream << anim->startValue().toReal();
                    dataStream << anim->endValue().toReal();
                }
                break;
            }
            case Bitmap::Type:
            {
                Bitmap *b = dynamic_cast<Bitmap *>(item);
                dataStream << Bitmap::Type;
                dataStream << b->id();
                dataStream << b->pos().x();
                dataStream << b->pos().y();
                dataStream << b->zValue();
                dataStream << b->rect().width();
                dataStream << b->rect().height();
                dataStream << b->getImage();
                dataStream << b->getAnimationCount();
                for(int i=0; i< b->getAnimationCount(); i++)
                {
                    QPropertyAnimation *anim = b->getAnimation(i);
                    dataStream << anim->property("begin").toInt();
                    dataStream << anim->property("min").toInt();
                    dataStream << anim->property("max").toInt();
                    dataStream << anim->duration();
                    dataStream << QString(anim->propertyName());
                    dataStream << anim->startValue().toReal();
                    dataStream << anim->endValue().toReal();
                }
                break;
            }
            case Vectorgraphic::Type:
            {
                Vectorgraphic *v = dynamic_cast<Vectorgraphic *>(item);
                dataStream << Vectorgraphic::Type;
                dataStream << v->id();
                dataStream << v->pos().x();
                dataStream << v->pos().y();
                dataStream << v->zValue();
                dataStream << v->xscale();
                dataStream << v->yscale();
                dataStream << v->getData();
                dataStream << v->getAnimationCount();
                for(int i=0; i< v->getAnimationCount(); i++)
                {
                    QPropertyAnimation *anim = v->getAnimation(i);
                    dataStream << anim->property("begin").toInt();
                    dataStream << anim->property("min").toInt();
                    dataStream << anim->property("max").toInt();
                    dataStream << anim->duration();
                    dataStream << QString(anim->propertyName());
                    dataStream << anim->startValue().toReal();
                    dataStream << anim->endValue().toReal();
                }
                break;
            }
            default:
                qWarning() << "undefined item type " << item->type();
                break;
        }
    }
    return dataStream;
}

void AnimationScene::addPropertyAnimationRequested(ResizeableItem *item, const QString propertyName, qreal value, int min, int max)
{
    emit addPropertyAnimation(item, propertyName, value, min, max);
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
            Rectangle *r = new Rectangle(m_copy->rect().width(), m_copy->rect().height());
            r->setPos(m_copy->pos().x() + 10, m_copy->pos().y() + 10);
            r->setId("");
            r->setPen(m_copy->pen());
            r->setBrush(m_copy->brush());
            r->setFlag(QGraphicsItem::ItemIsMovable, true);
            r->setFlag(QGraphicsItem::ItemIsSelectable, true);
            connect(r, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimationRequested(ResizeableItem *, const QString, qreal, int, int)));
            addItem(r);
            emit itemAdded(r);
            break;
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

QString getItemName(ResizeableItem *item)
{
    QString name = getItemTypeName(item);
    if(!item->id().isEmpty())
        name = item->id();
    return name;
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
