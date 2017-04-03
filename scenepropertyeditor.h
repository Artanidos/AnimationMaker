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
#include <QLabel>

#include "animationscene.h"
#include "colorpicker.h"
#include "colorrect.h"

class ScenePropertyEditor : public QWidget
{
    Q_OBJECT
public:
    ScenePropertyEditor();

    void setScene(AnimationScene *scene);
private:
    bool m_initializing;
    QSpinBox *m_width;
    QSpinBox *m_height;
    QSpinBox *m_fps;
    QLineEdit *m_color;
    AnimationScene *m_scene;
    ColorPicker *m_colorpicker;
    QSpinBox *m_hue;
    QSpinBox *m_saturation;
    QSpinBox *m_lightness;
    QSpinBox *m_red;
    QSpinBox *m_green;
    QSpinBox *m_blue;
    QLabel *m_labelHue;
    QLabel *m_labelSaturation;
    QLabel *m_labelLightness;
    QLabel *m_labelRed;
    QLabel *m_labelGreen;
    QLabel *m_labelBlue;
    QSlider *m_hueSlider;
    ColorRect *m_colorRect;

    void setColorParts(QColor value);

private slots:
    void widthChanged(int value);
    void heightChanged(int value);
    void fpsChanged(int value);
    void colorChanged(QString value);
    void colorChanged(QColor value);
    void hueChanged(int value);
    void hueValueChanged(int value);
    void saturationValueChanged(int value);
    void lightnessValueChanged(int value);
    void redValueChanged(int value);
    void greenValueChanged(int value);
    void blueValueChanged(int value);
    void colorRectClicked();
};

#endif // SCENEPROPERTYEDITOR_H
