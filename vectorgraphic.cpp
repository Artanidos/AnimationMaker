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

void Vectorgraphic::scaleObjects()
{
    m_xscale = rect().width() / m_svg->boundingRect().width();
    m_yscale = rect().height() / m_svg->boundingRect().height();

    QTransform trans;
    trans.scale(m_xscale, m_yscale);
    m_svg->setTransform(trans);
}
