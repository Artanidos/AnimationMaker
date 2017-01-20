#ifndef TEXT_H
#define TEXT_H

#include <QString>
#include "item.h"

class Text : public Item
{
public:
    Text();
    Text(int x, int y, QString text);

    virtual QString getTypeName();
    QDataStream& read(QDataStream &dataStream);
    QDataStream& write(QDataStream &dataStream) const;
    void paint(QPainter *painter);

    void setFont(QFont font);
    void setTextColor(QColor color);

private:
    QString m_text;
    QColor m_textColor;
    QFont m_font;
};

#endif // TEXT_H
