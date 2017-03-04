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

#ifndef SCENEPROPERTYEDITOR_H
#define SCENEPROPERTYEDITOR_H

#include <QSpinBox>
#include <QLineEdit>

#include "animationscene.h"
#include "colorpicker.h"

class ScenePropertyEditor : public QWidget
{
    Q_OBJECT
public:
    ScenePropertyEditor();

    void setScene(AnimationScene *scene);
private:
    QSpinBox *m_width;
    QSpinBox *m_height;
    QSpinBox *m_fps;
    QSpinBox *m_length;
    QLineEdit *m_color;
    AnimationScene *m_scene;
    ColorPicker *m_colorpicker;

private slots:
    void widthChanged(int value);
    void heightChanged(int value);
    void fpsChanged(int value);
    void lengthChanged(int value);
    void colorChanged(QString value);
    void colorChanged(QColor value);
    void hueChanged(int value);
};

#endif // SCENEPROPERTYEDITOR_H
