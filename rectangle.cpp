#include "rectangle.h"

#include <QtTest/QTest>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>

Rectangle::Rectangle(qreal width, qreal height)
    : ResizeableItem()
{
    setRect(0, 0, width, height);
}

int Rectangle::type() const
{
    return Rectangle::Type;
}

void Rectangle::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    paint->setPen(pen());
    paint->setBrush(brush());
    paint->drawRect(rect());

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}
