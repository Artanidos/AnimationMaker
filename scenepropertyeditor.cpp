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
    m_length = new QSpinBox();
    m_width->setMaximum(10000);
    m_height->setMaximum(10000);
    m_fps->setMaximum(60);
    m_length->setMinimum(0);
    m_length->setMaximum(10000);
    m_color = new QLineEdit();
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(new QLabel("FPS"), 0, 0);
    layout->addWidget(m_fps, 0, 1);
    layout->addWidget(new QLabel("Minimum length"), 1, 0);
    layout->addWidget(m_length, 1, 1);
    layout->addWidget(new QLabel("seconds"), 1, 2);
    layout->addWidget(new QLabel("Size"), 2, 0);
    layout->addWidget(new QLabel("W"), 2, 1);
    layout->addWidget(m_width, 2, 2);
    layout->addWidget(new QLabel("H"), 2, 3);
    layout->addWidget(m_height, 2, 4);
    layout->addWidget(new QLabel("BackgroundColor"), 3, 0);
    layout->addWidget(m_color, 3, 1, 1, 2);
    exp->addLayout(layout);
    vbox->addWidget(exp);
    vbox->addStretch();
    this->setLayout(vbox);

    connect(m_width, SIGNAL(valueChanged(int)), this, SLOT(widthChanged(int)));
    connect(m_height, SIGNAL(valueChanged(int)), this, SLOT(heightChanged(int)));
    connect(m_fps, SIGNAL(valueChanged(int)), this, SLOT(fpsChanged(int)));
    connect(m_length, SIGNAL(valueChanged(int)), this, SLOT(lengthChanged(int)));
    connect(m_color, SIGNAL(textChanged(QString)), this, SLOT(colorChanged(QString)));
}

void ScenePropertyEditor::setScene(AnimationScene *scene)
{
    m_scene = scene;
    m_width->setValue(m_scene->width());
    m_height->setValue(m_scene->height());
    m_fps->setValue(m_scene->fps());
    m_length->setValue(m_scene->length());
    m_color->setText(m_scene->backgroundColor().name());
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

void ScenePropertyEditor::lengthChanged(int value)
{
    m_scene->setLength(value);
}

void ScenePropertyEditor::colorChanged(QString value)
{
    m_scene->setBackgroundColor(QColor(value));
}
