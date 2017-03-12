#include "transitionline.h"
#include "keyframehandle.h"

#include <QPainter>

TransitionLine::TransitionLine(ResizeableItem *item, QString propertyName)
{
    m_item = item;
    m_propertyName = propertyName;

    setMaximumHeight(14);
    setMinimumHeight(14);

    m_imageRaute = QImage(":/images/raute-weiss.png");
    m_imageRauteHohl = QImage(":/images/raute-hohl.png");
}

void TransitionLine::paintEvent(QPaintEvent *)
{
    QColor gray;
    if(m_propertyName.isEmpty())
        gray = QColor(76, 78, 80);
    else
        gray = QColor(64, 66, 68);
    int width = size().width();
    int height = size().height();

    QPainter painter(this);
    painter.fillRect(0, 0, width, height, gray);

    painter.setPen(QColor(41, 41, 41));
    for(int k = 199; k < width; k+=200)
    {
        painter.drawLine(k, 0, k, height);
    }

    if(!m_propertyName.isEmpty())
    {
        QList<KeyFrame *> *list = m_item->keyframes()->value(m_propertyName);
        QList<KeyFrame *>::iterator it;
        for(it = list->begin(); it != list->end(); it++)
        {
            KeyFrame *frame = *it;
            painter.drawImage(frame->time() / 5 - 6, 1, m_imageRaute);
        }
    }
}
