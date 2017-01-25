#include "ellipse.h"

#include <QtTest/QTest>

Ellipse::Ellipse(qreal width, qreal height)
    : QGraphicsEllipseItem(0,0,width, height)
{

}

int Ellipse::type() const
{
    return Ellipse::Type;
}

void Ellipse::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsEllipseItem::paint(paint, option, widget);
}

QRectF Ellipse::boundingRect() const
{
    return rect();
}

bool Ellipse::sceneEventFilter(QGraphicsItem * watched, QEvent * event)
{
    ItemHandle * handle = dynamic_cast<ItemHandle *>(watched);
    if ( handle == NULL)
        return false;

    QGraphicsSceneMouseEvent * mevent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if ( mevent == NULL)
    {
        return false;
    }

    switch (event->type() )
    {
    case QEvent::GraphicsSceneMousePress:
    {
        handle->setMouseState(ItemHandle::kMouseDown);
        handle->mouseDownX = mevent->pos().x();
        handle->mouseDownY = mevent->pos().y();
    }
        break;

    case QEvent::GraphicsSceneMouseRelease:
        handle->setMouseState(ItemHandle::kMouseReleased);
        break;

    case QEvent::GraphicsSceneMouseMove:
        handle->setMouseState(ItemHandle::kMouseMoving );
        break;

    default:
        return false;
    }

    if ( handle->getMouseState() == ItemHandle::kMouseMoving )
    {
        qreal x = mevent->pos().x(), y = mevent->pos().y();

        int XaxisSign = 0;
        int YaxisSign = 0;
        switch( handle->getCorner() )
        {
        case 0:
        {
            XaxisSign = +1;
            YaxisSign = +1;
        }
            break;

        case 1:
        {
            XaxisSign = -1;
            YaxisSign = +1;
        }
            break;

        case 2:
        {
            XaxisSign = -1;
            YaxisSign = -1;
        }
            break;

        case 3:
        {
            XaxisSign = +1;
            YaxisSign = -1;
        }
            break;

        case 4:
        {
            YaxisSign = +1;
        }
            break;
        case 5:
        {
            XaxisSign = -1;
        }
            break;

        case 6:
        {
            YaxisSign = -1;
        }
            break;
        case 7:
        {
            XaxisSign = +1;

        }
            break;
        }

        int xMoved = handle->mouseDownX - x;
        int yMoved = handle->mouseDownY - y;

        int newWidth = rect().width() + ( XaxisSign * xMoved);
        if ( newWidth < 40 ) newWidth  = 40;

        int newHeight = rect().height() + (YaxisSign * yMoved) ;
        if ( newHeight < 40 ) newHeight = 40;

        int deltaWidth  =   newWidth - rect().width() ;
        int deltaHeight =   newHeight - rect().height() ;

        setRect(0,0,rect().width() + deltaWidth, rect().height() + deltaHeight);

        deltaWidth *= (-1);
        deltaHeight *= (-1);

        switch(handle->getCorner())
        {
            case 0:
            {
                int newXpos = this->pos().x() + deltaWidth;
                int newYpos = this->pos().y() + deltaHeight;
                this->setPos(newXpos, newYpos);
            }
            break;

            case 1:
            {
                int newYpos = this->pos().y() + deltaHeight;
                this->setPos(this->pos().x(), newYpos);
            }
            break;

            case 3:
            {
                int newXpos = this->pos().x() + deltaWidth;
                this->setPos(newXpos, this->pos().y());
            }
            break;

            case 4:
            {
                int newYpos = this->pos().y() + deltaHeight;
                this->setPos(this->pos().x(), newYpos);
            }
            break;

            case 7:
            {
                int newXpos = this->pos().x() + deltaWidth;
                this->setPos(newXpos, this->pos().y());
            }
            break;
        }

        setHandlePositions();

        this->update();
    }
    return true;
}

void Ellipse::setHandlePositions()
{
    m_handles[0]->setPos(-4, -4);
    m_handles[1]->setPos(rect().width() - 4,  -4);
    m_handles[2]->setPos(rect().width() - 4, rect().height() - 4);
    m_handles[3]->setPos(-4,  rect().height() - 4);
    m_handles[4]->setPos(rect().width() / 2 - 4, -4);
    m_handles[5]->setPos(rect().width() - 4,  rect().height() / 2 - 4);
    m_handles[6]->setPos(rect().width() /2 - 4, rect().height() - 4);
    m_handles[7]->setPos(-4,  rect().height() / 2 - 4);
}

QVariant Ellipse::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
    {
        if (value == true)
        {
            if(!m_hasHandles)
            {
                for(int i = 0; i < 8; i++)
                {
                    m_handles[i] = new ItemHandle(this,i);
                    m_handles[i]->installSceneEventFilter(this);
                }
                setHandlePositions();
                m_hasHandles = true;
            }
        }
        else
        {
            for(int i = 0; i < 8; i++)
            {
                m_handles[i]->setParentItem(NULL);
                delete m_handles[i];
            }
            m_hasHandles = false;
        }
    }

    return QGraphicsItem::itemChange(change, value);
}
