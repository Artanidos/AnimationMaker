#include "vectorgraphic.h"

#include <QtTest/QTest>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>

Vectorgraphic::Vectorgraphic(QString filename)
    : ResizeableItem()
{
    m_svg = new QGraphicsSvgItem(filename, this);

    setRect(0, 0, m_svg->boundingRect().width(), m_svg->boundingRect().height());
}

int Vectorgraphic::type() const
{
    return Vectorgraphic::Type;
}

void Vectorgraphic::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}

void Vectorgraphic::setScale(qreal x, qreal y)
{
    m_xscale = x;
    m_yscale = y;
    QTransform trans;
    trans.scale(m_xscale, m_yscale);
    m_svg->setTransform(trans);

    setRect(0, 0, m_svg->boundingRect().width() * x, m_svg->boundingRect().height() * y);
}

qreal Vectorgraphic::xscale()
{
    return m_xscale;
}

qreal Vectorgraphic::yscale()
{
    return m_yscale;
}

bool Vectorgraphic::sceneEventFilter(QGraphicsItem * watched, QEvent * event)
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

        m_xscale = rect().width() / m_svg->boundingRect().width();
        m_yscale = rect().height() / m_svg->boundingRect().height();

        QTransform trans;
        trans.scale(m_xscale, m_yscale);
        m_svg->setTransform(trans);

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
