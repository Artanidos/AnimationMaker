#include "animationscene.h"
#include "serializeableitem.h"
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
        QGraphicsRectItem *r = addRect(0, 0, 50, 50, QPen(Qt::black), QBrush(Qt::blue));
        r->setFlag(QGraphicsItem::ItemIsMovable, true);
        r->setFlag(QGraphicsItem::ItemIsSelectable, true);
        r->setPos(mouseEvent->scenePos());
    }
    else if(m_editMode == EditMode::ModeEllipse)
    {
        QGraphicsEllipseItem *e = addEllipse(0, 0, 50, 50, QPen(Qt::black), QBrush(Qt::blue));
        e->setFlag(QGraphicsItem::ItemIsMovable, true);
        e->setFlag(QGraphicsItem::ItemIsSelectable, true);
        e->setPos(mouseEvent->scenePos());
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
    qreal x, y, z, width, height;
    QPen pen;
    QBrush brush;

    clear();
    dataStream >> width;
    dataStream >> height;

    while (!dataStream.atEnd())
    {
        dataStream >> type;
        if(type == QGraphicsRectItem::Type)
        {
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> width;
            dataStream >> height;
            dataStream >> pen;
            dataStream >> brush;
            QGraphicsRectItem *r = addRect(0, 0, width, height, pen, brush);
            r->setPos(x, y);
            r->setFlag(QGraphicsItem::ItemIsMovable, true);
            r->setFlag(QGraphicsItem::ItemIsSelectable, true);
            r->setZValue(z);
        }
        else if(type == QGraphicsEllipseItem::Type)
        {
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> width;
            dataStream >> height;
            dataStream >> pen;
            dataStream >> brush;
            QGraphicsEllipseItem *e = addEllipse(0, 0, width, height, pen, brush);
            e->setPos(x, y);
            e->setFlag(QGraphicsItem::ItemIsMovable, true);
            e->setFlag(QGraphicsItem::ItemIsSelectable, true);
            e->setZValue(z);
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
        case QGraphicsRectItem::Type:
        {
            QGraphicsRectItem *r = dynamic_cast<QGraphicsRectItem *>(item);
            dataStream << QGraphicsRectItem::Type;
            dataStream << r->pos().x();
            dataStream << r->pos().y();
            dataStream << r->zValue();
            dataStream << r->rect().width();
            dataStream << r->rect().height();
            dataStream << r->pen();
            dataStream << r->brush();
            break;
        }
        case QGraphicsEllipseItem::Type:
        {
            QGraphicsEllipseItem *e = dynamic_cast<QGraphicsEllipseItem *>(item);
            dataStream << QGraphicsEllipseItem::Type;
            dataStream << e->pos().x();
            dataStream << e->pos().y();
            dataStream << e->zValue();
            dataStream << e->rect().width();
            dataStream << e->rect().height();
            dataStream << e->pen();
            dataStream << e->brush();
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
