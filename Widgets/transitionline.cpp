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
#include "commands.h"
#include "keyframehandle.h"
#include "keyframe.h"
#include "transition.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QUndoCommand>
#include <QUndoStack>
#include <QTest>

TransitionLine::TransitionLine(AnimationItem *item, QString propertyName, Timeline *timeline, QUndoStack *undostack)
{
    m_item = item;
    m_frame = nullptr;
    m_selectedFrame = nullptr;
    m_propertyName = propertyName;
    m_playheadPosition = 0;
    m_timeline = timeline;
    m_undostack = undostack;

    setMaximumHeight(18);
    setMinimumHeight(18);

    connect(item, SIGNAL(positionChanged(qreal,qreal)), this, SLOT(update()));
    connect(item, SIGNAL(sizeChanged(qreal,qreal)), this, SLOT(update()));
    connect(item, SIGNAL(opacityChanged(int)), this, SLOT(update()));
    if(!m_propertyName.isEmpty())
    {
        connect(item, SIGNAL(keyframeAdded(QString,KeyFrame*)), this, SLOT(addKeyframe(QString,KeyFrame*)));
        connect(item, SIGNAL(keyframeRemoved(QString,KeyFrame*)), this, SLOT(removeKeyframe(QString,KeyFrame*)));
        connect(item, SIGNAL(transitionRemoved(QString,KeyFrame*)), this, SLOT(removeTransition(QString,KeyFrame*)));
    }
}

void TransitionLine::paintEvent(QPaintEvent *)
{
    QColor orange(255, 127, 42, 150);
    QColor orangeSelected(255, 127, 42, 255);
    QColor color;
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
    painter.drawLine(0, 0, 0, height);
    for(int k = 200 - offset; k < width; k+=200)
    {
        painter.drawLine(k, 0, k, height);
    }
    painter.drawLine(0, height - 1, width, height - 1);

    painter.setPen(Qt::red);
    painter.drawLine(m_playheadPosition / 5 - offset, 0, m_playheadPosition / 5 - offset, height);
}

void TransitionLine::addKeyframe(QString propertyName, KeyFrame *key)
{
    if(propertyName != m_propertyName)
        return;

    if(key->prev() && key->prev()->easing() > -1)
    {
        Transition *trans = new Transition(this, key->prev(), m_timeline, m_undostack);
        trans->move(key->prev()->time() / 5 - m_horizontalScrollValue * 20,0);
        connect(trans, SIGNAL(transitionMoved(Transition*,int)), this, SLOT(moveTransition(Transition*,int)));
        connect(trans, SIGNAL(transitionResized()), this, SLOT(transitionResized()));
        trans->installEventFilter(this);
        // no need to add a keyframehandle when a transition exists
        // but we have to remove the prev keyframehandle
        removeKeyframe(propertyName, key->prev());
        return;
    }
    if(key->easing() > -1)
    {
        // no need to add a keyframehandle when a transition exists
        // transition will be added with next keyframe
        return;
    }
    KeyframeHandle *handle = new KeyframeHandle(this, key);
	connect(handle, SIGNAL(keyframeDeleted(KeyframeHandle*)), this, SLOT(deleteKeyframe(KeyframeHandle*)));
    connect(handle, SIGNAL(transitionAdded(KeyFrame*)), this, SLOT(addTransition(KeyFrame*)));
    connect(handle, SIGNAL(keyframeMoved(KeyframeHandle*,int)), this, SLOT(moveKeyframe(KeyframeHandle*,int)));
    handle->move(key->time() / 5 - m_horizontalScrollValue * 20 - 6, 2);
}

void TransitionLine::removeKeyframe(QString propertyName, KeyFrame *key)
{
    if(propertyName != m_propertyName)
        return;
    QList<KeyframeHandle*> handles = findChildren<KeyframeHandle*>();
    foreach(KeyframeHandle *handle, handles)
    {
        if(handle->key() == key)
        {
            delete handle;
            update();
        }
    }
}

void TransitionLine::removeTransition(QString propertyName, KeyFrame *key)
{
    if(propertyName != m_propertyName)
        return;
    QList<Transition*> transitions = findChildren<Transition*>();
    foreach(Transition *transition, transitions)
    {
        if(transition->key() == key)
        {
            delete transition;
            update();
        }
    }
    if(!key->prev() || key->prev()->easing() == -1)
    {
        KeyframeHandle *handleLeft = new KeyframeHandle(this, key);
        connect(handleLeft, SIGNAL(keyframeDeleted(KeyframeHandle*)), this, SLOT(deleteKeyframe(KeyframeHandle*)));
        connect(handleLeft, SIGNAL(transitionAdded(KeyFrame*)), this, SLOT(addTransition(KeyFrame*)));
        connect(handleLeft, SIGNAL(keyframeMoved(KeyframeHandle*,int)), this, SLOT(moveKeyframe(KeyframeHandle*,int)));
        handleLeft->move(key->time() / 5 - m_horizontalScrollValue * 20 - 6, 2);
    }
    if(!key->next()->next() || key->next()->easing() == -1)
    {
        KeyframeHandle *handleRight = new KeyframeHandle(this, key->next());
        connect(handleRight, SIGNAL(keyframeDeleted(KeyframeHandle*)), this, SLOT(deleteKeyframe(KeyframeHandle*)));
        connect(handleRight, SIGNAL(transitionAdded(KeyFrame*)), this, SLOT(addTransition(KeyFrame*)));
        connect(handleRight, SIGNAL(keyframeMoved(KeyframeHandle*,int)), this, SLOT(moveKeyframe(KeyframeHandle*,int)));
        handleRight->move(key->next()->time() / 5 - m_horizontalScrollValue * 20 - 6, 2);
    }
}

void TransitionLine::setScrollValue(int value)
{
    m_horizontalScrollValue = value;

    QList<KeyframeHandle*> handles = findChildren<KeyframeHandle*>();
    foreach(KeyframeHandle *handle, handles)
    {
        handle->move(handle->key()->time() / 5 - m_horizontalScrollValue * 20 - 6, 2);
    }
    QList<Transition*> transitions = findChildren<Transition*>();
    foreach(Transition *trans, transitions)
    {
        trans->move(trans->key()->time() / 5 - m_horizontalScrollValue * 20, 0);
    }
    update();
}

KeyframeHandle *TransitionLine::getKeyframeHandle(KeyFrame *key)
{
    QList<KeyframeHandle*> handles = findChildren<KeyframeHandle*>();
    foreach(KeyframeHandle *handle, handles)
    {
        if(handle->key() == key)
            return handle;
    }
    return nullptr;
}

Transition *TransitionLine::getTransition(KeyFrame *key)
{
    QList<Transition*> transitions = findChildren<Transition*>();
    foreach(Transition *transition, transitions)
    {
        if(transition->key() == key)
            return transition;
    }
    return nullptr;
}

void TransitionLine::deleteKeyframe(KeyframeHandle *handle)
{
    emit keyframeDeleted(m_item, m_propertyName, handle->key());
}

void TransitionLine::addTransition(KeyFrame *key)
{
    emit transitionAdded(m_item, m_propertyName, key);
}

void TransitionLine::addTransitionGui(KeyFrame *key)
{
    Transition *trans = new Transition(this, key, m_timeline, m_undostack);
    trans->move(key->time() / 5 - m_horizontalScrollValue * 20,0);
    connect(trans, SIGNAL(transitionMoved(Transition*,int)), this, SLOT(moveTransition(Transition*,int)));
    connect(trans, SIGNAL(transitionResized()), this, SLOT(transitionResized()));
    trans->installEventFilter(this);

    // remove keyframe handles
    QList<KeyframeHandle*> handles = findChildren<KeyframeHandle*>();
    for (auto it = handles.rbegin(); it != handles.rend(); ++it)
    {
        KeyframeHandle *hand = *it;
        if(hand->key() == key || hand->key()->prev() == key)
        {
            delete hand;
        }
    }
}

void TransitionLine::moveKeyframe(KeyframeHandle *handle, int time)
{
    if(time >= 0
            && handle->key()->time() != time
            && (handle->key()->next() == nullptr || handle->key()->next()->time() >= time)
            && (handle->key()->prev() == nullptr || handle->key()->prev()->time() <= time))
    {
        QUndoCommand *cmd = new MoveKeyframeCommand(handle->key(), handle->key()->time(), time, m_timeline);
        m_undostack->push(cmd);
    }
}

void TransitionLine::moveTransition(Transition *transition, int time)
{
    if(time >= 0
            && transition->key()->time() != time
            && (transition->key()->next()->next() == nullptr
                 || (transition->key()->next()->easing() > -1 && transition->key()->next()->next()->time() - 100 >= transition->key()->next()->time() - transition->key()->time() + time)
                 || (transition->key()->next()->easing() < 0 && transition->key()->next()->next()->time() >= transition->key()->next()->time() - transition->key()->time() + time))
            && (transition->key()->prev() == nullptr
                || (transition->key()->prev()->easing() > -1 && transition->key()->prev()->time() + 100 <= time)
                || (transition->key()->prev()->easing() < 0 && transition->key()->prev()->time() <= time)))
    {
        QUndoCommand *cmd = new MoveTransitionCommand(transition->key(), transition->key()->time(), time, m_timeline);
        m_undostack->push(cmd);
    }
}

void TransitionLine::transitionResized()
{
    update();
}

bool TransitionLine::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        Transition *t = dynamic_cast<Transition*>(object);
        if(t)
            emit m_timeline->transitionSelectionChanged(t->key());
    }
    return false;
}
