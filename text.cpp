#include "text.h"

#include <QTest>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>

Text::Text(QString text)
    : ResizeableItem()
{
   m_font = QFont("Arial", 13);
   m_text = text;
   QFontMetrics m(m_font);
   setRect(0, 0, m.width(m_text), m.height());
   m_textitem = new QGraphicsSimpleTextItem(m_text, this);
   m_textitem->setFont(m_font);
   setPen(QPen(QBrush(), 0));
}

int Text::type() const
{
    return Text::Type;
}

void Text::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if (option->state & QStyle::State_Selected)
        drawHighlightSelected(paint, option);
}

QString Text::text()
{
    return m_text;
}

void Text::setScale(qreal x, qreal y)
{
    m_xscale = x;
    m_yscale = y;
    QTransform trans;
    trans.scale(m_xscale, m_yscale);
    m_textitem->setTransform(trans);

    QFontMetrics m(m_font);
    setRect(0, 0, m.width(m_text) * x, m.height() * y);
}

void Text::scaleObjects()
{
    QFontMetrics m(m_font);
    m_xscale = rect().width() / m.width(m_text);
    m_yscale = rect().height() / m.height();

    QTransform trans;
    trans.scale(m_xscale, m_yscale);
    m_textitem->setTransform(trans);
}
