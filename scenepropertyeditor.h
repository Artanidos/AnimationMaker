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
