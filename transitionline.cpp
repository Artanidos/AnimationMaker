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

    setMouseTracking(true);

    setMaximumHeight(14);
    setMinimumHeight(14);

    m_imageRaute = QImage(":/images/raute-weiss.png");
    m_imageRauteHohl = QImage(":/images/raute-hohl.png");
    m_imageLeft = QImage(":/images/trans-left.png");
    m_imageRight = QImage(":/images/trans-right.png");

    m_contextMenu = new QMenu();
    m_transitionAct = new QAction("Create transition");
    m_delKeyframeAct = new QAction("Delete keyframe");
    m_delTransitionAct = new QAction("Delete transition");
    connect(m_transitionAct, SIGNAL(triggered(bool)), this, SLOT(addTransaction()));
    connect(m_delKeyframeAct, SIGNAL(triggered(bool)), this, SLOT(deleteKeyframe()));
    connect(m_delTransitionAct, SIGNAL(triggered(bool)), this, SLOT(deleteTransition()));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    connect(item, SIGNAL(positionChanged(qreal,qreal)), this, SLOT(update()));
    connect(item, SIGNAL(sizeChanged(qreal,qreal)), this, SLOT(update()));
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
    int offset = m_horizontalScrollValue * 20;

    QPainter painter(this);
    painter.fillRect(0, 0, width, height, gray);

    painter.setPen(QColor(41, 41, 41));
    for(int k = 199 - offset; k < width; k+=200)
    {
        painter.drawLine(k, 0, k, height);
    }

    if(!m_propertyName.isEmpty())
    {
        KeyFrame *first = m_item->keyframes()->value(m_propertyName);
        for(KeyFrame *frame = first; frame != NULL; frame = frame->next())
        {
            if(frame->easing() >= 0)
            {
                painter.fillRect(frame->time() / 5 - offset, 1, (frame->next()->time() - frame->time()) / 5, height - 1, orange);
                painter.drawImage(frame->time() / 5 - offset, 1, m_imageLeft);
                painter.drawImage(frame->next()->time() / 5 - 5 - offset, 1, m_imageRight);
            }
            else if(frame->prev() == NULL || frame->prev()->easing() < 0)
            {
                if(frame->prev() == NULL || frame->prev()->value() == frame->value())
                    painter.drawImage(frame->time() / 5 - 6 - offset, 2, m_imageRaute);
                else
                    painter.drawImage(frame->time() / 5 - 6 - offset, 2, m_imageRauteHohl);
            }
        }
    }

    painter.setPen(Qt::red);
    painter.drawLine(m_playheadPosition / 5 - 1 - offset, 0, m_playheadPosition / 5 - 1 - offset, height);
}

void TransitionLine::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        if(!m_propertyName.isEmpty())
        {
            int offset = m_horizontalScrollValue * 20;
            KeyFrame *first = m_item->keyframes()->value(m_propertyName);
            for(KeyFrame *frame = first; frame != NULL; frame = frame->next())
            {
                int pos = frame->time() / 5 - 6;
                if(ev->pos().x() + offset >= pos && ev->pos().x() + offset <= pos + 11)
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
    bool isOverKeyframe = false;

    if(m_pressed)
    {   
        int x = ev->x();
        if(x < 0)
            x = 0;
        if(x >= width())
            x = width() - 1;
        x += m_horizontalScrollValue * 20;
        int time = qRound((qreal)x * 5 / 100) * 100;
        if(time / 5 > width())
            time -= 100;
        m_frame->setTime(time);
        m_oldx = ev->pos().x();
        update();
    }

    if(!m_propertyName.isEmpty())
    {
        int offset = m_horizontalScrollValue * 20;
        KeyFrame *first = m_item->keyframes()->value(m_propertyName);
        for(KeyFrame *frame = first; frame != NULL; frame = frame->next())
        {
            int pos = frame->time() / 5 - 6;
            if(ev->pos().x() + offset >= pos && ev->pos().x() + offset <= pos + 11)
            {
                isOverKeyframe = true;
                break;
            }
        }
    }
    if(isOverKeyframe)
        setCursor(Qt::SizeHorCursor);
    else
        setCursor(Qt::ArrowCursor);
}

void TransitionLine::mouseReleaseEvent(QMouseEvent *)
{
    m_pressed = false;
}

void TransitionLine::onCustomContextMenu(const QPoint &point)
{
    KeyFrame *first = m_item->keyframes()->value(m_propertyName);
    for(KeyFrame *frame = first; frame != NULL; frame = frame->next())
    {
        int pos = frame->time() / 5 - 6;
        if(point.x() + m_horizontalScrollValue * 20 >= pos && point.x() + m_horizontalScrollValue * 20 <= pos + 11)
        {
            m_frame = frame;
            m_contextMenu->clear();
            m_contextMenu->addAction(m_delKeyframeAct);
            if(frame->prev() && frame->prev()->easing() < 0)
                m_contextMenu->addAction(m_transitionAct);
            m_contextMenu->exec(this->mapToGlobal(point));
            break;
        }
        if(point.x() + m_horizontalScrollValue * 20 > pos + 11 && frame->next() && frame->easing() >=0 && point.x() + m_horizontalScrollValue * 20 < frame->next()->time() / 5 - 6)
        {
            m_frame = frame;
            m_contextMenu->clear();
            m_contextMenu->addAction(m_delTransitionAct);
            m_contextMenu->exec(this->mapToGlobal(point));
            break;
        }
    }
}

void TransitionLine::addTransaction()
{
    m_frame->prev()->setEasing((int)QEasingCurve::Linear);
    update();
}

void TransitionLine::deleteKeyframe()
{
    if(m_frame->next())
        m_frame->next()->setPrev(m_frame->prev());
    if(m_frame->prev())
        m_frame->prev()->setNext(m_frame->next());
    else
    {
        m_item->keyframes()->remove(m_propertyName);
        if(m_frame->next())
            m_item->keyframes()->insert(m_propertyName, m_frame->next());
        else
            emit keyframeDeleted(m_item, m_propertyName);
    }
    delete m_frame;
    m_frame = NULL;
    update();
}

void TransitionLine::deleteTransition()
{
    m_frame->setEasing(-1);
    update();
}
