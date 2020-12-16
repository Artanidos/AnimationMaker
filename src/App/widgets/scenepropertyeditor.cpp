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
#include "commands.h"

#include <QGridLayout>
#include <QLabel>

ScenePropertyEditor::ScenePropertyEditor()
{
    QVBoxLayout *vbox = new QVBoxLayout();
    Expander *exp = new Expander("Scene");
    m_colorEditor = new ColorEditor("Backgroundcolor");
    m_width = new QSpinBox();
    m_height = new QSpinBox();
    m_fps = new QSpinBox();
    m_width->setMaximum(10000);
    m_height->setMaximum(10000);
    m_fps->setMaximum(60);
    QLabel *labelWidth = new QLabel("W");
    QLabel *labelHeight = new QLabel("H");
    labelWidth->setFixedWidth(15);
    labelHeight->setFixedWidth(15);
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(new QLabel("FPS"), 0, 0);
    layout->addWidget(m_fps, 0, 2);
    layout->addWidget(new QLabel("Size"), 1, 0);
    layout->addWidget(labelWidth, 1, 1);
    layout->addWidget(m_width, 1, 2);
    layout->addWidget(labelHeight, 1, 3);
    layout->addWidget(m_height, 1, 4);
    layout->addWidget(m_colorEditor, 2, 0, 1, 5);
    exp->addLayout(layout);
    vbox->addWidget(exp);
    vbox->addStretch();
    this->setLayout(vbox);

    connect(m_width, SIGNAL(valueChanged(int)), this, SLOT(widthChanged(int)));
    connect(m_height, SIGNAL(valueChanged(int)), this, SLOT(heightChanged(int)));
    connect(m_fps, SIGNAL(valueChanged(int)), this, SLOT(fpsChanged(int)));
    connect(m_colorEditor, SIGNAL(colorChanged(QColor)), this, SLOT(colorChanged(QColor)));
    connect(m_colorEditor, SIGNAL(addKeyframe()), this, SLOT(addKeyFrame()));
 }

void ScenePropertyEditor::setScene(AnimationScene *scene)
{
    m_scene = scene;
    m_width->setValue(m_scene->width());
    m_height->setValue(m_scene->height());
    m_fps->setValue(m_scene->fps());
    m_colorEditor->setColor(m_scene->backgroundColor());
    connect(m_scene, SIGNAL(backgroundColorChanged(QColor)), this, SLOT(backgroundColorChanged(QColor)));
}

void ScenePropertyEditor::backgroundColorChanged(QColor color)
{
    m_colorEditor->setColor(color);
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

void ScenePropertyEditor::colorChanged(QColor color)
{
    QUndoCommand *cmd = new ChangeColorCommand(color, m_scene->backgroundColor(), m_scene, m_scene->backgroundRect());
    m_scene->undoStack()->push(cmd);
}

void ScenePropertyEditor::addKeyFrame()
{
    emit addKeyFrame(m_scene->backgroundRect(), "brushColor", QVariant(m_colorEditor->color()));
}
