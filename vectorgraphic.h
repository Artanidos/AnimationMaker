#ifndef VECTORGRAPHIC_H
#define VECTORGRAPHIC_H

#include <QGraphicsItem>
#include <QGraphicsSvgItem>
#include "resizeableitem.h"

class Vectorgraphic : public ResizeableItem
{
public:
    Vectorgraphic(QString filename, QMenu *menu);
    Vectorgraphic(QByteArray arr, QMenu *menu);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    void scaleObjects();
    void setScale(qreal x, qreal y);
    QByteArray getData();

    enum { Type = UserType + 5 };
    int type() const Q_DECL_OVERRIDE;

private:
    QGraphicsSvgItem *m_svg;
    QSvgRenderer *m_renderer;
    QByteArray m_data;
};

#endif // VECTORGRAPHIC_H
