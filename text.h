#ifndef TEXT_H
#define TEXT_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include "itemhandle.h"
#include "resizeableitem.h"

class Text : public ResizeableItem
{
public:
    Text(QString text);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    bool sceneEventFilter(QGraphicsItem * watched, QEvent * event);
    void setScale(qreal x, qreal y);
    qreal xscale();
    qreal yscale();
    QString text();

    enum { Type = UserType + 3 };
    int type() const Q_DECL_OVERRIDE;

private:
    qreal m_xscale;
    qreal m_yscale;
    QFont m_font;
    QString m_text;
    QGraphicsSimpleTextItem *m_textitem;
};

#endif // TEXT_H
