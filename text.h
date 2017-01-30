#ifndef TEXT_H
#define TEXT_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include "itemhandle.h"

class Text : public QGraphicsRectItem
{
public:
    Text(QString text);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    QRectF boundingRect() const;
    bool sceneEventFilter(QGraphicsItem * watched, QEvent * event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void setScale(qreal x, qreal y);
    qreal xscale();
    qreal yscale();
    QString text();

    enum { Type = UserType + 3 };
    int type() const Q_DECL_OVERRIDE;

private:
    ItemHandle*  m_handles[8];
    bool m_hasHandles;
    qreal m_xscale;
    qreal m_yscale;
    QFont m_font;
    QString m_text;
    QGraphicsSimpleTextItem *m_textitem;

    void setHandlePositions();
};

#endif // TEXT_H
