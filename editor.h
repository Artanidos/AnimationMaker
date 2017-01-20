#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include "scene.h"

class Editor : public QWidget
{
    //Q_OBJECT
public:
    Editor(QWidget *parent = 0);
    QSize sizeHint() const;
    void setScene(Scene *scene);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Scene *m_scene;
};

#endif // EDITOR_H
