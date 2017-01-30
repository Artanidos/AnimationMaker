#ifndef VECTORGRAPHIC_H
#define VECTORGRAPHIC_H

#include <QGraphicsItem>
#include <QGraphicsSvgItem>
#include "resizeableitem.h"

class Vectorgraphic : public ResizeableItem
{
public:
    Vectorgraphic(QString filename);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    bool sceneEventFilter(QGraphicsItem * watched, QEvent * event);
    void setScale(qreal x, qreal y);
    qreal xscale();
    qreal yscale();
    QString text();

    enum { Type = UserType + 5 };
    int type() const Q_DECL_OVERRIDE;

private:
    QGraphicsSvgItem *m_svg;
    qreal m_xscale;
    qreal m_yscale;
};

#endif // VECTORGRAPHIC_H
