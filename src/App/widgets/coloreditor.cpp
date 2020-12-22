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

#include "coloreditor.h"

#include <QGridLayout>

ColorEditor::ColorEditor(QString label)
{
    QGridLayout *layout = new QGridLayout();
    m_color = new QLineEdit();
    m_colorPicker = new ColorPicker();
    m_colorPicker->setVisible(false);
    m_hueSlider = new QSlider();
    m_hueSlider->setVisible(false);
    m_hueSlider->setMinimum(0);
    m_hueSlider->setMaximum(100);
    m_hueSlider->setOrientation(Qt::Vertical);
    m_hueSlider->setMaximumHeight(100);
    m_rect = new ColorRect();
    m_hue = new QSpinBox();
    m_saturation = new QSpinBox();
    m_lightness = new QSpinBox();
    m_labelHue = new QLabel("H");
    m_labelSaturation = new QLabel("S");
    m_labelLightness = new QLabel("L");
    m_red = new QSpinBox();
    m_green = new QSpinBox();
    m_blue = new QSpinBox();
    m_labelRed = new QLabel("R");
    m_labelGreen = new QLabel("G");
    m_labelBlue = new QLabel("B");
    m_hue->setVisible(false);
    m_saturation->setVisible(false);
    m_lightness->setVisible(false);
    m_red->setVisible(false);
    m_green->setVisible(false);
    m_blue->setVisible(false);
    m_labelHue->setVisible(false);
    m_labelSaturation->setVisible(false);
    m_labelLightness->setVisible(false);
    m_labelRed->setVisible(false);
    m_labelGreen->setVisible(false);
    m_labelBlue->setVisible(false);
    m_red->setMinimum(0);
    m_red->setMaximum(255);
    m_green->setMinimum(0);
    m_green->setMaximum(255);
    m_blue->setMinimum(0);
    m_blue->setMaximum(255);
    m_hue->setMinimum(0);
    m_hue->setMaximum(100);
    m_saturation->setMinimum(0);
    m_saturation->setMaximum(100);
    m_lightness->setMinimum(0);
    m_lightness->setMaximum(100);
    m_labelHue->setFixedWidth(15);
    m_labelSaturation->setFixedWidth(15);
    m_labelLightness->setFixedWidth(15);
    m_labelRed->setFixedWidth(15);
    m_labelGreen->setFixedWidth(15);
    m_labelBlue->setFixedWidth(15);
    QLabel *l = new QLabel(label);
    l->setMinimumWidth(100);
    m_addKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    m_addKeyframe->setMaximumWidth(9);
    m_addKeyframe->setToolTip("Add keyframe for Left");

    layout->addWidget(l, 0, 0);
    layout->addWidget(m_addKeyframe, 0, 1);
    layout->addWidget(m_rect, 0, 2);
    layout->addWidget(m_color, 0, 3, 1, 4);

    layout->addWidget(m_colorPicker, 1, 0, 3, 2);
    layout->addWidget(m_hueSlider, 1, 2, 3, 1);
    layout->addWidget(m_labelHue, 1, 3);
    layout->addWidget(m_hue, 1, 4);
    layout->addWidget(m_labelSaturation, 2, 3);
    layout->addWidget(m_saturation, 2, 4);
    layout->addWidget(m_labelLightness, 3, 3);
    layout->addWidget(m_lightness, 3, 4);
    layout->addWidget(m_labelRed, 1, 5);
    layout->addWidget(m_red, 1, 6);
    layout->addWidget(m_labelGreen, 2, 5);
    layout->addWidget(m_green, 2, 6);
    layout->addWidget(m_labelBlue, 3, 5);
    layout->addWidget(m_blue, 3, 6);

    setLayout(layout);

    connectControls();
}

void ColorEditor::setColor(QColor color)
{
    setColorParts(color);
}

void ColorEditor::disconnectControls()
{
    disconnect(m_color, SIGNAL(editingFinished()), this, SLOT(colorTextChanged()));
    disconnect(m_hueSlider, SIGNAL(valueChanged(int)), this, SLOT(hueChanged(int)));
    disconnect(m_hueSlider, SIGNAL(sliderReleased()), this, SLOT(huePicked()));
    disconnect(m_colorPicker, SIGNAL(colorChanged(QColor)), this, SLOT(colorPickerChanged(QColor)));
    disconnect(m_colorPicker, SIGNAL(colorPicked(QColor)), this, SLOT(colorPicked(QColor)));
    disconnect(m_rect, SIGNAL(mouseClicked()), this, SLOT(rectClicked()));
    disconnect(m_hue, SIGNAL(valueChanged(int)), this, SLOT(hueValueChanged(int)));
    disconnect(m_saturation, SIGNAL(valueChanged(int)), this, SLOT(saturationValueChanged(int)));
    disconnect(m_lightness, SIGNAL(valueChanged(int)), this, SLOT(lightnessValueChanged(int)));
    disconnect(m_red, SIGNAL(valueChanged(int)), this, SLOT(redValueChanged(int)));
    disconnect(m_green, SIGNAL(valueChanged(int)), this, SLOT(greenValueChanged(int)));
    disconnect(m_blue, SIGNAL(valueChanged(int)), this, SLOT(blueValueChanged(int)));
    disconnect(m_addKeyframe, SIGNAL(clicked()), this, SLOT(addKeyframeClicked()));
}

void ColorEditor::connectControls()
{
    connect(m_color, SIGNAL(editingFinished()), this, SLOT(colorTextChanged()));
    connect(m_hueSlider, SIGNAL(valueChanged(int)), this, SLOT(hueChanged(int)));
    connect(m_hueSlider, SIGNAL(sliderReleased()), this, SLOT(huePicked()));
    connect(m_colorPicker, SIGNAL(colorChanged(QColor)), this, SLOT(colorPickerChanged(QColor)));
    connect(m_colorPicker, SIGNAL(colorPicked(QColor)), this, SLOT(colorPicked(QColor)));
    connect(m_rect, SIGNAL(mouseClicked()), this, SLOT(rectClicked()));
    connect(m_hue, SIGNAL(valueChanged(int)), this, SLOT(hueValueChanged(int)));
    connect(m_saturation, SIGNAL(valueChanged(int)), this, SLOT(saturationValueChanged(int)));
    connect(m_lightness, SIGNAL(valueChanged(int)), this, SLOT(lightnessValueChanged(int)));
    connect(m_red, SIGNAL(valueChanged(int)), this, SLOT(redValueChanged(int)));
    connect(m_green, SIGNAL(valueChanged(int)), this, SLOT(greenValueChanged(int)));
    connect(m_blue, SIGNAL(valueChanged(int)), this, SLOT(blueValueChanged(int)));
    connect(m_addKeyframe, SIGNAL(clicked()), this, SLOT(addKeyframeClicked()));
}

void ColorEditor::rectClicked()
{
    setExpanded(!m_colorPicker->isVisible());
}

void ColorEditor::setExpanded(bool value)
{
    m_colorPicker->setVisible(value);
    m_hueSlider->setVisible(value);
    m_labelHue->setVisible(value);
    m_labelSaturation->setVisible(value);
    m_labelLightness->setVisible(value);
    m_hue->setVisible(value);
    m_saturation->setVisible(value);
    m_lightness->setVisible(value);
    m_labelRed->setVisible(value);
    m_labelGreen->setVisible(value);
    m_labelBlue->setVisible(value);
    m_red->setVisible(value);
    m_green->setVisible(value);
    m_blue->setVisible(value);
}

bool ColorEditor::isExpanded()
{
    return m_colorPicker->isVisible();
}

void ColorEditor::setColorParts(QColor value)
{
    disconnectControls();
    m_hue->setValue(value.hslHueF() * 100);
    m_hueSlider->setValue(value.hslHueF() * 100);
    m_colorPicker->setHue(value.hslHueF());
    m_saturation->setValue(value.hslSaturationF() * 100);
    m_lightness->setValue(value.lightnessF() * 100);
    m_red->setValue(value.red());
    m_green->setValue(value.green());
    m_blue->setValue(value.blue());
    m_color->setText(value.name());
    m_rect->setColor(value);
    connectControls();
}

void ColorEditor::colorTextChanged()
{
    QColor color = QColor(m_color->text());
    setColorParts(color);
    emit colorChanged(color);
}

void ColorEditor::colorPicked(QColor value)
{
    setColorParts(value);
    emit colorChanged(value);
}

void ColorEditor::colorPickerChanged(QColor value)
{
    setColorParts(value);
}

void ColorEditor::hueChanged(int value)
{
    m_colorPicker->setHue((qreal)value / 100.0);
    QColor color = QColor::fromHslF((qreal)value / 100, (qreal)m_saturation->value() / 100, (qreal)m_lightness->value() / 100, 1.0);
    setColorParts(color);
}

void ColorEditor::hueValueChanged(int value)
{
    m_hueSlider->setValue(value);
    QColor color = QColor::fromHslF((qreal)value / 100, (qreal)m_saturation->value() / 100, (qreal)m_lightness->value() / 100, 1.0);
    setColorParts(color);
    emit colorChanged(color);
}

void ColorEditor::huePicked()
{
    int value = m_hueSlider->value();
    m_colorPicker->setHue((qreal)value / 100.0);
    QColor color = QColor::fromHslF((qreal)value / 100, (qreal)m_saturation->value() / 100, (qreal)m_lightness->value() / 100, 1.0);
    setColorParts(color);
    emit colorChanged(color);
}

void ColorEditor::addKeyframeClicked()
{
    emit addKeyframe();
}

void ColorEditor::saturationValueChanged(int value)
{
    QColor color = QColor::fromHslF((qreal)m_hue->value() / 100, (qreal)value / 100, (qreal)m_lightness->value() / 100, 1.0);
    setColorParts(color);
    emit colorChanged(color);
}

void ColorEditor::lightnessValueChanged(int value)
{
    QColor color = QColor::fromHslF((qreal)m_hue->value() / 100, (qreal)m_saturation->value() / 100, (qreal)value / 100, 1.0);
    setColorParts(color);
    emit colorChanged(color);
}

void ColorEditor::redValueChanged(int value)
{
    QColor color = QColor::fromRgb(value, m_green->value(), m_blue->value());
    setColorParts(color);
    emit colorChanged(color);
}

void ColorEditor::greenValueChanged(int value)
{
    QColor color = QColor::fromRgb(m_red->value(), value, m_blue->value());
    setColorParts(color);
    emit colorChanged(color);
}

void ColorEditor::blueValueChanged(int value)
{
    QColor color = QColor::fromRgb(m_red->value(), m_green->value(), value);
    setColorParts(color);
    emit colorChanged(color);
}
