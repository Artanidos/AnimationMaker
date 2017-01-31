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
    void scaleObjects();

    enum { Type = UserType + 5 };
    int type() const Q_DECL_OVERRIDE;

private:
    QGraphicsSvgItem *m_svg;
};

#endif // VECTORGRAPHIC_H
