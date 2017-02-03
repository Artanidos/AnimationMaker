#include "ellipse.h"

#include <QtTest/QTest>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>

Ellipse::Ellipse(qreal width, qreal height, QMenu *menu)
    : ResizeableItem(menu)
{
    setRect(0, 0, width, height);
}

int Ellipse::type() const
{
    return Ellipse::Type;
}

void Ellipse::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    paint->setPen(pen());
    paint->setBrush(brush());
    paint->drawEllipse(rect());

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}
