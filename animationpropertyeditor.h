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

#ifndef ANIMATIONPROPERTYEDITOR_H
#define ANIMATIONPROPERTYEDITOR_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QIntValidator>

class AnimationPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    AnimationPropertyEditor();

    void setAnimation(QPropertyAnimation *anim);

public slots:
    void beginChanged(int);
    void durationChanged(int);
    void fromChanged(int);
    void toChanged(int);
    void easingChanged(int);

signals:
    void dataChanged();

private:
    QPropertyAnimation *m_animation;
    QLineEdit *m_property;
    QSpinBox *m_begin;
    QSpinBox *m_duration;
    QSpinBox *m_from;
    QSpinBox *m_to;
    QComboBox *m_easing;
};

#endif // ANIMATIONPROPERTYEDITOR_H
