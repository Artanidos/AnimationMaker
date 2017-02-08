#include "animationscene.h"
#include "serializeableitem.h"
#include "handleitem.h"
#include "rectangle.h"
#include "ellipse.h"
#include "text.h"
#include "bitmap.h"
#include "vectorgraphic.h"

AnimationScene::AnimationScene(QMenu *menu)
{
    m_editMode = EditMode::ModeSelect;
    m_itemMenu = menu;
}

void AnimationScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    if(m_editMode == EditMode::ModeSelect)
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
        if(!this->selectedItems().isEmpty())
        {
            ResizeableItem *item =  dynamic_cast<ResizeableItem *>(this->selectedItems().first());
            if(item)
                emit itemSelectionChanged(item);
        }
    }
    else if(m_editMode == EditMode::ModeRectangle)
    {
        clearSelection();

        Rectangle *r = new Rectangle(50, 50, m_itemMenu);
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
        clearSelection();

        Ellipse *e = new Ellipse(50, 50, m_itemMenu);
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
        clearSelection();

        Text *t = new Text("Lorem ipsum dolor", m_itemMenu);
        t->setFlag(QGraphicsItem::ItemIsMovable, true);
        t->setFlag(QGraphicsItem::ItemIsSelectable, true);
        t->setPos(mouseEvent->scenePos());
        addItem(t);
        emit itemAdded(t);
    }
    else if(m_editMode == EditMode::ModeBitmap)
    {
        clearSelection();

        QString fileName = QFileDialog::getOpenFileName(0, tr("Open Bitmap"), "", tr("Image Files (*.png *.jpeg *.jpg *.gif *.bmp);;All Files (*)"));
        if (fileName.isEmpty())
            return;

        Bitmap *b = new Bitmap(fileName, m_itemMenu);
        b->setFlag(QGraphicsItem::ItemIsMovable, true);
        b->setFlag(QGraphicsItem::ItemIsSelectable, true);
        b->setPos(mouseEvent->scenePos());
        addItem(b);
        emit itemAdded(b);
    }
    else if(m_editMode == EditMode::ModeSvg)
    {
        clearSelection();

        QString fileName = QFileDialog::getOpenFileName(0, tr("Open SVG"), "", tr("SVG Files (*.svg);;All Files (*)"));
        if (fileName.isEmpty())
            return;

        Vectorgraphic *v = new Vectorgraphic(fileName, m_itemMenu);
        v->setFlag(QGraphicsItem::ItemIsMovable, true);
        v->setFlag(QGraphicsItem::ItemIsSelectable, true);
        v->setPos(mouseEvent->scenePos());
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

            Rectangle *r = new Rectangle(width, height, m_itemMenu);
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
            Ellipse *e = new Ellipse(width, height, m_itemMenu);
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
            Text *t = new Text(text, m_itemMenu);
            t->setPos(x, y);
            t->setFlag(QGraphicsItem::ItemIsMovable, true);
            t->setFlag(QGraphicsItem::ItemIsSelectable, true);
            t->setZValue(z);
            t->setScale(xscale, yscale);
            addItem(t);
        }
        else if(type == Bitmap::Type)
        {
            QImage img;
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> width;
            dataStream >> height;
            dataStream >> img;
            Bitmap *b = new Bitmap(img, width, height, m_itemMenu);
            b->setPos(x, y);
            b->setFlag(QGraphicsItem::ItemIsMovable, true);
            b->setFlag(QGraphicsItem::ItemIsSelectable, true);
            b->setZValue(z);
            b->setScale(xscale, yscale);
            addItem(b);
        }
        else if(type == Vectorgraphic::Type)
        {
            QByteArray arr;
            dataStream >> x;
            dataStream >> y;
            dataStream >> z;
            dataStream >> xscale;
            dataStream >> yscale;
            dataStream >> arr;
            Vectorgraphic *v = new Vectorgraphic(arr, m_itemMenu);
            v->setPos(x, y);
            v->setFlag(QGraphicsItem::ItemIsMovable, true);
            v->setFlag(QGraphicsItem::ItemIsSelectable, true);
            v->setZValue(z);
            v->setScale(xscale, yscale);
            addItem(v);
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
            case Bitmap::Type:
            {
                Bitmap *b = dynamic_cast<Bitmap *>(item);
                dataStream << Bitmap::Type;
                dataStream << b->pos().x();
                dataStream << b->pos().y();
                dataStream << b->zValue();
                dataStream << b->rect().width();
                dataStream << b->rect().height();
                dataStream << b->getImage();
                break;
            }
            case Vectorgraphic::Type:
            {
                Vectorgraphic *v = dynamic_cast<Vectorgraphic *>(item);
                dataStream << Vectorgraphic::Type;
                dataStream << v->pos().x();
                dataStream << v->pos().y();
                dataStream << v->zValue();
                dataStream << v->xscale();
                dataStream << v->yscale();
                dataStream << v->getData();
                break;
            }
            default:
                qWarning() << "undefined item type " << item->type();
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

QString getItemTypeName(QGraphicsItem *item)
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
