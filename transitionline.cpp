/****************************************************************************
** Copyright (C) 2016 Olaf Japp
**
** This file is part of AnimationMaker.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "transitionline.h"
#include "keyframehandle.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QTest>

TransitionLine::TransitionLine(ResizeableItem *item, QString propertyName)
{
    m_item = item;
    m_frame = NULL;
    m_propertyName = propertyName;
    m_playheadPosition = 0;
    m_pressed = false;

    setMaximumHeight(14);
    setMinimumHeight(14);

    m_imageRaute = QImage(":/images/raute-weiss.png");
    m_imageRauteHohl = QImage(":/images/raute-hohl.png");
    m_imageLeft = QImage(":/images/trans-left.png");
    m_imageRight = QImage(":/images/trans-right.png");

    m_contextMenu = new QMenu();
    m_transitionAct = new QAction("Create transition");
    connect(m_transitionAct, SIGNAL(triggered(bool)), this, SLOT(addTransaction()));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
}

void TransitionLine::paintEvent(QPaintEvent *)
{
    QColor orange(255, 102, 0, 150);
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
            if(frame->easing() >= 0)
            {
                painter.fillRect(frame->time() / 5, 1, (frame->transitionTo()->time() - frame->time()) / 5, height - 1, orange);
                painter.drawImage(frame->time() / 5, 1, m_imageLeft);
                painter.drawImage(frame->transitionTo()->time() / 5 - 5, 1, m_imageRight);
            }
            else if(!frame->transitionFrom())
                painter.drawImage(frame->time() / 5 - 6, 2, m_imageRaute);
        }
    }

    painter.setPen(Qt::red);
    painter.drawLine(m_playheadPosition / 5 - 1, 0, m_playheadPosition / 5 - 1, height);
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
                    m_pressed = true;
                    break;
                }
            }
        }
    }
}

void TransitionLine::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_pressed)
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
    m_pressed = false;
}

void TransitionLine::onCustomContextMenu(const QPoint &point)
{
    QList<KeyFrame *> *list = m_item->keyframes()->value(m_propertyName);
    QList<KeyFrame *>::iterator it;
    for(it = list->begin(); it != list->end(); it++)
    {
        KeyFrame *frame = *it;
        int pos = frame->time() / 5 - 6;
        if(point.x() >= pos && point.x() <= pos + 11)
        {
            m_frame = frame;
            m_contextMenu->clear();
            m_contextMenu->addAction(m_transitionAct);
            m_contextMenu->exec(this->mapToGlobal(point));

            break;
        }
    }
}

void TransitionLine::addTransaction()
{
    KeyFrame *prev = NULL;
    QList<KeyFrame*> *list = m_item->keyframes()->value(m_propertyName);
    QList<KeyFrame*>::iterator it;
    for(it = list->begin(); it != list->end(); it++)
    {
        if(*it == m_frame)
            break;
        prev = *it;
    }
    if(prev)
    {
        prev->setEasing((int)QEasingCurve::Linear);
        prev->setTransitionTo(m_frame);
        m_frame->setTransitionFrom(prev);
        update();
    }
}
