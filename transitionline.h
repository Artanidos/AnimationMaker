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

    inline QString propertyName() {return m_propertyName;}
    inline ResizeableItem *item() {return m_item;}

private:
    QImage m_imageRaute;
    QImage m_imageRauteHohl;
    ResizeableItem *m_item;
    QString m_propertyName;
};

#endif // TRANSITION_H
