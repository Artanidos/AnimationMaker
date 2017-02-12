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
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(new QLabel("FPS"), 0, 0);
    layout->addWidget(m_fps, 0, 1);
    layout->addWidget(new QLabel("Size"), 1, 0);
    layout->addWidget(new QLabel("W"), 1, 1);
    layout->addWidget(m_width, 1, 2);
    layout->addWidget(new QLabel("H"), 1, 3);
    layout->addWidget(m_height, 1, 4);
    exp->addLayout(layout);
    vbox->addWidget(exp);
    vbox->addStretch();
    this->setLayout(vbox);

    connect(m_width, SIGNAL(valueChanged(int)), this, SLOT(widthChanged(int)));
    connect(m_height, SIGNAL(valueChanged(int)), this, SLOT(heightChanged(int)));
    connect(m_fps, SIGNAL(valueChanged(int)), this, SLOT(fpsChanged(int)));
}

 void ScenePropertyEditor::setScene(AnimationScene *scene)
 {
     m_scene = scene;
     m_width->setValue(m_scene->width());
     m_height->setValue(m_scene->height());
     m_fps->setValue(m_scene->fps());
 }

 void ScenePropertyEditor::widthChanged(int value)
 {
    m_scene->setSceneRect(0, 0, value, m_scene->height());
 }

 void ScenePropertyEditor::heightChanged(int value)
 {
     m_scene->setSceneRect(0, 0, m_scene->width(), value);
 }

 void ScenePropertyEditor::fpsChanged(int value)
 {
     m_scene->setFps(value);
 }
