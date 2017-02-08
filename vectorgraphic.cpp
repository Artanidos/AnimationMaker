#include "vectorgraphic.h"

#include <QtTest/QTest>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>
#include <QSvgRenderer>

Vectorgraphic::Vectorgraphic(QString filename)
    : ResizeableItem()
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    m_data = file.readAll();
    m_renderer = new QSvgRenderer();
    m_renderer->load(m_data);
    m_svg = new QGraphicsSvgItem(this);
    m_svg->setSharedRenderer(m_renderer);

    setRect(0, 0, m_svg->boundingRect().width(), m_svg->boundingRect().height());
}

Vectorgraphic::Vectorgraphic(QByteArray arr)
    : ResizeableItem()
{
    m_data = arr;
    m_renderer = new QSvgRenderer();
    m_renderer->load(m_data);
    m_svg = new QGraphicsSvgItem(this);
    m_svg->setSharedRenderer(m_renderer);

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

void Vectorgraphic::scaleObjects()
{
    m_xscale = rect().width() / m_svg->boundingRect().width();
    m_yscale = rect().height() / m_svg->boundingRect().height();

    QTransform trans;
    trans.scale(m_xscale, m_yscale);
    m_svg->setTransform(trans);
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


QByteArray Vectorgraphic::getData()
{
    return m_data;
}
