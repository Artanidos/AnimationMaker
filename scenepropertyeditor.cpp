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

#include "scenepropertyeditor.h"
#include "expander.h"

#include <QGridLayout>
#include <QLabel>

ScenePropertyEditor::ScenePropertyEditor()
{
    QVBoxLayout *vbox = new QVBoxLayout();
    Expander *exp = new Expander("Scene");
    m_width = new QSpinBox();
    m_height = new QSpinBox();
    m_fps = new QSpinBox();
    m_width->setMaximum(10000);
    m_height->setMaximum(10000);
    m_fps->setMaximum(60);
    m_color = new QLineEdit();
    m_colorpicker = new ColorPicker();
    m_colorpicker->setVisible(false);
    m_colorRect = new ColorRect();
    m_hueSlider = new QSlider();
    m_hueSlider->setVisible(false);
    m_hueSlider->setMinimum(0);
    m_hueSlider->setMaximum(100);
    m_hueSlider->setOrientation(Qt::Vertical);
    m_hueSlider->setMaximumHeight(100);
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

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(new QLabel("FPS"), 0, 0);
    layout->addWidget(m_fps, 0, 2);
    layout->addWidget(new QLabel("Size"), 1, 0);
    layout->addWidget(new QLabel("W"), 1, 1);
    layout->addWidget(m_width, 1, 2);
    layout->addWidget(new QLabel("H"), 1, 3);
    layout->addWidget(m_height, 1, 4);
    layout->addWidget(new QLabel("BackgroundColor"), 2, 0);
    layout->addWidget(m_colorRect, 2, 1);
    layout->addWidget(m_color, 2, 2, 1, 4);
    layout->addWidget(m_colorpicker, 3, 0, 3, 1);
    layout->addWidget(m_hueSlider, 3, 1, 3, 1);
    layout->addWidget(m_labelHue, 3, 2);
    layout->addWidget(m_hue, 3, 3);
    layout->addWidget(m_labelSaturation, 4, 2);
    layout->addWidget(m_saturation, 4, 3);
    layout->addWidget(m_labelLightness, 5, 2);
    layout->addWidget(m_lightness, 5, 3);
    layout->addWidget(m_labelRed, 3, 4);
    layout->addWidget(m_red, 3, 5);
    layout->addWidget(m_labelGreen, 4, 4);
    layout->addWidget(m_green, 4, 5);
    layout->addWidget(m_labelBlue, 5, 4);
    layout->addWidget(m_blue, 5, 5);

    exp->addLayout(layout);
    vbox->addWidget(exp);
    vbox->addStretch();
    this->setLayout(vbox);

    connect(m_width, SIGNAL(valueChanged(int)), this, SLOT(widthChanged(int)));
    connect(m_height, SIGNAL(valueChanged(int)), this, SLOT(heightChanged(int)));
    connect(m_fps, SIGNAL(valueChanged(int)), this, SLOT(fpsChanged(int)));
    connect(m_color, SIGNAL(textChanged(QString)), this, SLOT(colorChanged(QString)));
    connect(m_colorRect, SIGNAL(mouseClicked()), this, SLOT(colorRectClicked()));
    connect(m_hueSlider, SIGNAL(valueChanged(int)), this, SLOT(hueChanged(int)));
    connect(m_colorpicker, SIGNAL(colorChanged(QColor)), this, SLOT(colorChanged(QColor)));
    connect(m_hue, SIGNAL(valueChanged(int)), this, SLOT(hueValueChanged(int)));
    connect(m_saturation, SIGNAL(valueChanged(int)), this, SLOT(saturationValueChanged(int)));
    connect(m_lightness, SIGNAL(valueChanged(int)), this, SLOT(lightnessValueChanged(int)));
    connect(m_red, SIGNAL(valueChanged(int)), this, SLOT(redValueChanged(int)));
    connect(m_green, SIGNAL(valueChanged(int)), this, SLOT(greenValueChanged(int)));
    connect(m_blue, SIGNAL(valueChanged(int)), this, SLOT(blueValueChanged(int)));
}

void ScenePropertyEditor::setScene(AnimationScene *scene)
{
    m_scene = scene;
    m_width->setValue(m_scene->width());
    m_height->setValue(m_scene->height());
    m_fps->setValue(m_scene->fps());
    m_color->setText(m_scene->backgroundColor().name());
    setColorParts(m_scene->backgroundColor());
}

void ScenePropertyEditor::setColorParts(QColor value)
{
    m_initializing = true;
    m_hue->setValue(value.hslHueF() * 100);
    m_hueSlider->setValue(value.hslHueF() * 100);
    m_saturation->setValue(value.hslSaturationF() * 100);
    m_lightness->setValue(value.lightnessF() * 100);
    m_red->setValue(value.red());
    m_green->setValue(value.green());
    m_blue->setValue(value.blue());
    m_initializing = false;
}

void ScenePropertyEditor::widthChanged(int value)
{
    m_scene->setWidth(value);
}

void ScenePropertyEditor::heightChanged(int value)
{
    m_scene->setHeight(value);
}

void ScenePropertyEditor::fpsChanged(int value)
{
    m_scene->setFps(value);
}

void ScenePropertyEditor::colorChanged(QString value)
{
    QColor color = QColor(value);
    m_scene->setBackgroundColor(color);
    m_colorRect->setColor(color);
    setColorParts(color);
}

void ScenePropertyEditor::colorChanged(QColor value)
{
    m_scene->setBackgroundColor(value);
    m_color->setText(value.name());
    m_colorRect->setColor(value);
    setColorParts(value);
}

void ScenePropertyEditor::hueChanged(int value)
{
    m_colorpicker->setHue((qreal)value / 100.0);
    m_hue->setValue(value);
}

void ScenePropertyEditor::hueValueChanged(int value)
{
    if(m_initializing)
        return;
    m_hueSlider->setValue(value);
    QColor color = QColor::fromHslF((qreal)value / 100, (qreal)m_saturation->value() / 100, (qreal)m_lightness->value() / 100, 1.0);
    m_color->setText(color.name());
    m_colorRect->setColor(color);
}

void ScenePropertyEditor::saturationValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromHslF((qreal)m_hue->value() / 100, (qreal)value / 100, (qreal)m_lightness->value() / 100, 1.0);
    m_color->setText(color.name());
    m_colorRect->setColor(color);
}

void ScenePropertyEditor::lightnessValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromHslF((qreal)m_hue->value() / 100, (qreal)m_saturation->value() / 100, (qreal)value / 100, 1.0);
    m_color->setText(color.name());
    m_colorRect->setColor(color);
}

void ScenePropertyEditor::redValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(value, m_green->value(), m_blue->value());
    m_color->setText(color.name());
    m_colorRect->setColor(color);
}

void ScenePropertyEditor::greenValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(m_red->value(), value, m_blue->value());
    m_color->setText(color.name());
    m_colorRect->setColor(color);
}

void ScenePropertyEditor::blueValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(m_red->value(), m_green->value(), value);
    m_color->setText(color.name());
    m_colorRect->setColor(color);
}

void ScenePropertyEditor::colorRectClicked()
{
    bool visible = m_colorpicker->isVisible();

    m_colorpicker->setVisible(!visible);
    m_hueSlider->setVisible(!visible);
    m_labelHue->setVisible(!visible);
    m_labelSaturation->setVisible(!visible);
    m_labelLightness->setVisible(!visible);
    m_hue->setVisible(!visible);
    m_saturation->setVisible(!visible);
    m_lightness->setVisible(!visible);
    m_labelRed->setVisible(!visible);
    m_labelGreen->setVisible(!visible);
    m_labelBlue->setVisible(!visible);
    m_red->setVisible(!visible);
    m_green->setVisible(!visible);
    m_blue->setVisible(!visible);
}
