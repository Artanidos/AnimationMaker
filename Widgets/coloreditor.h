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

#ifndef COLOREDITOR_H
#define COLOREDITOR_H

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include "colorpicker.h"
#include "colorrect.h"
#include "widgets_global.h"

class WIDGETSSHARED_EXPORT ColorEditor : public QWidget
{
    Q_OBJECT
public:
    ColorEditor(QString label);

    void setExpanded(bool value);
    bool isExpanded();
    void setColor(QColor color);

signals:
    void colorChanged(QColor value);

private:
    ColorPicker *m_colorPicker;
    QSlider *m_hueSlider;
    QSpinBox *m_hue;
    QSpinBox *m_saturation;
    QSpinBox *m_lightness;
    QSpinBox *m_red;
    QSpinBox *m_green;
    QSpinBox *m_blue;
    QLineEdit *m_color;
    ColorRect *m_rect;
    QLabel *m_labelHue;
    QLabel *m_labelSaturation;
    QLabel *m_labelLightness;
    QLabel *m_labelRed;
    QLabel *m_labelGreen;
    QLabel *m_labelBlue;

    void setColorParts(QColor value);
    void connectControls();
    void disconnectControls();

private slots:
    void colorTextChanged();
    void colorPickerChanged(QColor value);
    void colorPicked(QColor value);
    void rectClicked();
    void hueValueChanged(int value);
    void saturationValueChanged(int value);
    void lightnessValueChanged(int value);
    void redValueChanged(int value);
    void greenValueChanged(int value);
    void blueValueChanged(int value);
    void hueChanged(int);
    void huePicked();
};

#endif // COLOREDITOR_H
