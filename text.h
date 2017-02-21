#ifndef TEXT_H
#define TEXT_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QColor>
#include "itemhandle.h"
#include "resizeableitem.h"

class Text : public ResizeableItem
{
public:
    Text(QString text);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    void scaleObjects();
    void setScale(qreal x, qreal y);
    QString text();
    void setText(QString text);

    QColor textcolor();
    void setTextcolor(QColor textcolor);

    enum { Type = UserType + 3 };
    int type() const Q_DECL_OVERRIDE;

private:
    QFont m_font;
    QString m_text;
    QGraphicsSimpleTextItem *m_textitem;
    QColor m_textcolor;
};

#endif // TEXT_H
