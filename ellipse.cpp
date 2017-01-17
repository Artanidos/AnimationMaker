#include "ellipse.h"

#include <QPainter>
#include <QColor>

EllipseBorder::EllipseBorder(QObject *parent)
    : QObject(parent)
    , m_width(1)
    , m_color("#000000")
{
}

Ellipse::Ellipse(QQuickPaintedItem *parent)
    : QQuickPaintedItem(parent)
    , m_color("#000000")
{
}

void Ellipse::paint(QPainter *painter)
{
    painter->setPen(QPen(m_border.color(), m_border.width()));
    painter->setBrush( m_color);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->drawEllipse(boundingRect().adjusted(m_border.width() / 2, m_border.width() / 2, m_border.width() / 2 * -1, m_border.width() / 2 * -1));
}
