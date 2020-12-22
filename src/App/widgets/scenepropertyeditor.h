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

#include <QColor>
#include <QSpinBox>
#include <QWidget>

#include "animationitem.h"
#include "animationscene.h"
#include "coloreditor.h"

class ScenePropertyEditor : public QWidget
{
    Q_OBJECT
public:
    ScenePropertyEditor();

    void setScene(AnimationScene *scene);

signals:
    void addKeyFrame(AnimationItem*, QString, QVariant);

private:
    bool m_initializing;
    ColorEditor *m_colorEditor;
    QSpinBox *m_width;
    QSpinBox *m_height;
    QSpinBox *m_fps;
    AnimationScene *m_scene;

    void setColorParts(QColor value);

private slots:
    void widthChanged(int value);
    void heightChanged(int value);
    void fpsChanged(int value);
    void backgroundColorChanged(QColor color);
    void colorChanged(QColor color);
    void addKeyFrame();
};

#endif // SCENEPROPERTYEDITOR_H
