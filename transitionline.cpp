#include "transitionline.h"
#include "keyframehandle.h"

#include <QPainter>
#include <QMouseEvent>
#include <QTest>

TransitionLine::TransitionLine(ResizeableItem *item, QString propertyName)
{
    m_item = item;
    m_frame = NULL;
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

void TransitionLine::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        if(!m_propertyName.isEmpty())
        {
            QList<KeyFrame *> *list = m_item->keyframes()->value(m_propertyName);
            QList<KeyFrame *>::iterator it;
            for(it = list->begin(); it != list->end(); it++)
            {
                KeyFrame *frame = *it;
                int pos = frame->time() / 5 - 6;
                if(ev->pos().x() >= pos && ev->pos().x() <= pos + 11)
                {
                    m_oldx = ev->pos().x();
                    m_frame = frame;
                    break;
                }
            }
        }
    }
}

void TransitionLine::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_frame)
    {   
        int x = ev->x();
        if(x < 0)
            x = 0;
        if(x >= width())
            x = width() - 1;
        int time = qRound((qreal)x * 5 / 100) * 100;
        if(time / 5 > width())
            time -= 100;
        m_frame->setTime(time);
        m_oldx = ev->pos().x();
        update();
    }
}

void TransitionLine::mouseReleaseEvent(QMouseEvent *)
{
    m_frame = NULL;
}
