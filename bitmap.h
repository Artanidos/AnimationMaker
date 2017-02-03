#ifndef BITMAP_H
#define BITMAP_H

#include <QGraphicsItem>
#include "resizeableitem.h"

class Bitmap : public ResizeableItem
{
public:
    Bitmap(QString filename, QMenu *menu);
    Bitmap(QImage img, qreal width, qreal height, QMenu *menu);

    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);

    QImage getImage();

    enum { Type = UserType + 4 };
    int type() const Q_DECL_OVERRIDE;

private:
    QImage m_image;
};

#endif // BITMAP_H
