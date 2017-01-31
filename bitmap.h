#ifndef BITMAP_H
#define BITMAP_H

#include <QGraphicsItem>
#include "resizeableitem.h"

class Bitmap : public ResizeableItem
{
public:
    Bitmap(QString filename);
    Bitmap(QImage img, qreal width, qreal height);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);

    QImage getImage();

    enum { Type = UserType + 4 };
    int type() const Q_DECL_OVERRIDE;

private:
    QImage m_image;
};

#endif // BITMAP_H
