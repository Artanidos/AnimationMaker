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

#include "resizeableitem.h"

class KeyFrame
{
public:
    KeyFrame();

    inline void setItem(ResizeableItem *value) {m_item = value;}
    inline QString propertyName() {return m_propertyName;}
    inline void setPropertyName(QString value) {m_propertyName = value;}
    inline QVariant value() {return m_value;}
    inline void setValue(QVariant value) {m_value = value;}
    inline int time() {return m_time;}
    inline void setTime(int value) {m_time = value;}

private:
    ResizeableItem *m_item;
    QString m_propertyName;
    QVariant m_value;
    int m_time;
    QEasingCurve m_easing;
};
bool compareKeyframes (KeyFrame *a, KeyFrame *b);

#endif // KEYFRAME_H
