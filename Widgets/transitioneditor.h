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

#ifndef TRANSITIONEDITOR_H
#define TRANSITIONEDITOR_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QUndoStack>
#include "keyframe.h"

class TransitionEditor : public QWidget
{
    Q_OBJECT
public:
    TransitionEditor();

    void setKeyframe(KeyFrame *frame);
    inline void setUndoStack(QUndoStack *stack) {m_undoStack = stack;}

private slots:
    void listRowChanged(int row);
    void setCurve();
    void easingChanged(int easing);

private:
    bool m_initializing;
    QUndoStack *m_undoStack;
    KeyFrame *m_frame;
    QListWidget *m_easing;
    QListWidget *m_type;
    QLabel *m_curve;
    QPixmap m_linear;
    QPixmap m_inquad;
    QPixmap m_incubiq;
    QPixmap m_inquart;
    QPixmap m_inquint;
    QPixmap m_insine;
    QPixmap m_inexpo;
    QPixmap m_incirq;
    QPixmap m_inback;
    QPixmap m_inelastic;
    QPixmap m_inbounce;
    QPixmap m_outquad;
    QPixmap m_outcubiq;
    QPixmap m_outquart;
    QPixmap m_outquint;
    QPixmap m_outsine;
    QPixmap m_outexpo;
    QPixmap m_outcirq;
    QPixmap m_outback;
    QPixmap m_outelastic;
    QPixmap m_outbounce;
    QPixmap m_inoutquad;
    QPixmap m_inoutcubiq;
    QPixmap m_inoutquart;
    QPixmap m_inoutquint;
    QPixmap m_inoutsine;
    QPixmap m_inoutexpo;
    QPixmap m_inoutcirq;
    QPixmap m_inoutback;
    QPixmap m_inoutelastic;
    QPixmap m_inoutbounce;
    QPixmap m_outinquad;
    QPixmap m_outincubiq;
    QPixmap m_outinquart;
    QPixmap m_outinquint;
    QPixmap m_outinsine;
    QPixmap m_outinexpo;
    QPixmap m_outincirq;
    QPixmap m_outinback;
    QPixmap m_outinelastic;
    QPixmap m_outinbounce;

    void changeEasing(int);
};

#endif // TRANSITIONEDITOR_H
