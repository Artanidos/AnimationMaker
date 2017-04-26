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

#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <QVariant>

class KeyFrame : public QObject
{
    Q_OBJECT
public:
    KeyFrame();

    inline QVariant value() {return m_value;}
    inline void setValue(QVariant value) {m_value = value;}
    inline int time() {return m_time;}
    inline void setTime(int value) {m_time = value;}
    inline int easing() {return m_easing;}
    inline void setEasing(int value) {m_easing = value; emit easingChanged(value);}
    inline KeyFrame *next() {return m_next;}
    inline void setNext(KeyFrame *value) {m_next = value;}
    inline KeyFrame *prev() {return m_prev;}
    inline void setPrev(KeyFrame *value) {m_prev = value;}

signals:
    void easingChanged(int);

private:
    QVariant m_value;
    int m_time;
    int m_easing;
    KeyFrame *m_next;
    KeyFrame *m_prev;
};

#endif // KEYFRAME_H
