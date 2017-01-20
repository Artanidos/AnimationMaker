#include "text.h"

#include "scene.h"

Text::Text()
    : Text(0,0,"")
{
}

Text::Text(int x, int y, QString text)
    : Item(x, y)
{
    m_text = text;
}

QString Text::getTypeName()
{
    return QString("Text");
}

QDataStream& Text::read(QDataStream &dataStream)
{
    dataStream >> m_x;
    dataStream >> m_y;
    dataStream >> m_text;
    dataStream >> m_textColor;
    dataStream >> m_font;
    return dataStream;
}

QDataStream& Text::write(QDataStream &dataStream) const
{
    dataStream << (quint8)TEXT;
    dataStream << m_x;
    dataStream << m_y;
    dataStream << m_text;
    dataStream << m_textColor;
    dataStream << m_font;
    return dataStream;
}

void Text::setTextColor(QColor color)
{
    m_textColor = color;
}

void Text::setFont(QFont font)
{
    m_font = font;
}

void Text::paint(QPainter *painter)
{
    painter->setPen(m_textColor);
    painter->setFont(QFont(m_font));
    painter->drawText(m_x, m_y, m_text);
}

