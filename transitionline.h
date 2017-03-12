#ifndef TRANSITION_H
#define TRANSITION_H

#include <QWidget>
#include "resizeableitem.h"

class TransitionLine : public QWidget
{
    Q_OBJECT
public:
    TransitionLine(ResizeableItem *item, QString propertyName);

    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

    inline QString propertyName() {return m_propertyName;}
    inline ResizeableItem *item() {return m_item;}

private:
    QImage m_imageRaute;
    QImage m_imageRauteHohl;
    ResizeableItem *m_item;
    QString m_propertyName;
    KeyFrame *m_frame;
    int m_oldx;
};

#endif // TRANSITION_H
