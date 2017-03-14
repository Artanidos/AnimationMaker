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
    QSlider *hueSlider = new QSlider();
    hueSlider->setMinimum(0);
    hueSlider->setMaximum(100);
    hueSlider->setOrientation(Qt::Vertical);
    hueSlider->setMaximumHeight(100);
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(new QLabel("FPS"), 0, 0);
    layout->addWidget(m_fps, 0, 1);
    layout->addWidget(new QLabel("Size"), 1, 0);
    layout->addWidget(new QLabel("W"), 1, 1);
    layout->addWidget(m_width, 1, 2);
    layout->addWidget(new QLabel("H"), 1, 3);
    layout->addWidget(m_height, 1, 4);
    layout->addWidget(new QLabel("BackgroundColor"), 2, 0);
    layout->addWidget(m_color, 2, 1, 1, 2);
    layout->addWidget(m_colorpicker, 3, 0);
    layout->addWidget(hueSlider, 3, 1);
    exp->addLayout(layout);
    vbox->addWidget(exp);
    vbox->addStretch();
    this->setLayout(vbox);

    connect(m_width, SIGNAL(valueChanged(int)), this, SLOT(widthChanged(int)));
    connect(m_height, SIGNAL(valueChanged(int)), this, SLOT(heightChanged(int)));
    connect(m_fps, SIGNAL(valueChanged(int)), this, SLOT(fpsChanged(int)));
    connect(m_color, SIGNAL(textChanged(QString)), this, SLOT(colorChanged(QString)));
    connect(hueSlider, SIGNAL(valueChanged(int)), this, SLOT(hueChanged(int)));
    connect(m_colorpicker, SIGNAL(colorChanged(QColor)), this, SLOT(colorChanged(QColor)));
}

void ScenePropertyEditor::setScene(AnimationScene *scene)
{
    m_scene = scene;
    m_width->setValue(m_scene->width());
    m_height->setValue(m_scene->height());
    m_fps->setValue(m_scene->fps());
    m_color->setText(m_scene->backgroundColor().name());
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
    m_scene->setBackgroundColor(QColor(value));
}

void ScenePropertyEditor::hueChanged(int value)
{
    m_colorpicker->setHue((qreal)value / 100.0);
}

void ScenePropertyEditor::colorChanged(QColor value)
{
    m_scene->setBackgroundColor(value);
    m_color->setText(value.name());
}
