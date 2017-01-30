#include "animationscene.h"
#include "serializeableitem.h"
#include "handleitem.h"
#include "rectangle.h"
#include "ellipse.h"
#include "text.h"

#include <QGraphicsItem>

AnimationScene::AnimationScene()
{
    m_editMode = EditMode::ModeSelect;
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
        deselectAll();

        Rectangle *r = new Rectangle(50, 50);
        r->setPen(QPen(Qt::black));
        r->setBrush(QBrush(Qt::blue));
        r->setFlag(QGraphicsItem::ItemIsMovable, true);
        r->setFlag(QGraphicsItem::ItemIsSelectable, true);
        r->setPos(mouseEvent->scenePos());
        addItem(r);
        emit itemAdded(r);
    }
    else if(m_editMode == EditMode::ModeEllipse)
    {
        deselectAll();
        Ellipse *e = new Ellipse(50, 50);
        e->setPen(QPen(Qt::black));
        e->setBrush(QBrush(Qt::blue));
        e->setFlag(QGraphicsItem::ItemIsMovable, true);
        e->setFlag(QGraphicsItem::ItemIsSelectable, true);
        e->setPos(mouseEvent->scenePos());
        addItem(e);
        emit itemAdded(e);
    }
    else if(m_editMode == EditMode::ModeText)
    {
        deselectAll();

        Text *t = new Text("Lorem ipsum dolor");
        t->setFlag(QGraphicsItem::ItemIsMovable, true);
        t->setFlag(QGraphicsItem::ItemIsSelectable, true);
        t->setPos(mouseEvent->scenePos());
        addItem(t);
        emit itemAdded(t);
    }
}

void AnimationScene::deselectAll()
{
    QList<QGraphicsItem *> items = selectedItems();
    foreach( QGraphicsItem *item, items )
    {
        item->setSelected(false);
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
    int type;
    qreal x, y, z, width, height, xscale, yscale;
    QPen pen;
    QBrush brush;
    QString text;

    clear();
    dataStream >> width;
    dataStream >> height;

    while (!dataStream.atEnd())
    {
        dataStream >> type;
        if(type == Rectangle::Type)
        {
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> width;
            dataStream >> height;
            dataStream >> pen;
            dataStream >> brush;

            Rectangle *r = new Rectangle(width, height);
            r->setPos(x, y);
            r->setPen(pen);
            r->setBrush(brush);
            r->setFlag(QGraphicsItem::ItemIsMovable, true);
            r->setFlag(QGraphicsItem::ItemIsSelectable, true);
            r->setZValue(z);
            addItem(r);
        }
        else if(type == Ellipse::Type)
        {
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> width;
            dataStream >> height;
            dataStream >> pen;
            dataStream >> brush;
            Ellipse *e = new Ellipse(width, height);
            e->setPos(x, y);
            e->setPen(pen);
            e->setBrush(brush);
            e->setFlag(QGraphicsItem::ItemIsMovable, true);
            e->setFlag(QGraphicsItem::ItemIsSelectable, true);
            e->setZValue(z);
            addItem(e);
        }
        else if(type == Text::Type)
        {
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> xscale;
            dataStream >> yscale;
            dataStream >> text;
            Text *t = new Text(text);
            t->setPos(x, y);
            t->setFlag(QGraphicsItem::ItemIsMovable, true);
            t->setFlag(QGraphicsItem::ItemIsSelectable, true);
            t->setZValue(z);
            t->setScale(xscale, yscale);
            addItem(t);
        }
    }

    return dataStream;
}

QDataStream& AnimationScene::write(QDataStream &dataStream) const
{
    dataStream << width();
    dataStream << height();

    QList<QGraphicsItem*> itemList = items(Qt::AscendingOrder);
    foreach (QGraphicsItem *item, itemList)
    {
        switch(item->type())
        {
        case Rectangle::Type:
        {
            Rectangle *r = dynamic_cast<Rectangle *>(item);
            dataStream << Rectangle::Type;
            dataStream << r->pos().x();
            dataStream << r->pos().y();
            dataStream << r->zValue();
            dataStream << r->rect().width();
            dataStream << r->rect().height();
            dataStream << r->pen();
            dataStream << r->brush();
            break;
        }
        case Ellipse::Type:
        {
            Ellipse *e = dynamic_cast<Ellipse *>(item);
            dataStream << Ellipse::Type;
            dataStream << e->pos().x();
            dataStream << e->pos().y();
            dataStream << e->zValue();
            dataStream << e->rect().width();
            dataStream << e->rect().height();
            dataStream << e->pen();
            dataStream << e->brush();
            break;
        }
        case Text::Type:
        {
            Text *t = dynamic_cast<Text *>(item);
            dataStream << Text::Type;
            dataStream << t->pos().x();
            dataStream << t->pos().y();
            dataStream << t->zValue();
            dataStream << t->xscale();
            dataStream << t->yscale();
            dataStream << t->text();
            break;
        }
         default:
            printf("undefined item type %d\n", item->type());
            break;
        }
    }
    return dataStream;
}

QDataStream& operator <<(QDataStream &out, const AnimationScene *s)
{
    return s->write(out);
}

QDataStream& operator >>(QDataStream &in, AnimationScene *s)
{
    return s->read(in);
}
