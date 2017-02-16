#ifndef ANIMATIONPROPERTYEDITOR_H
#define ANIMATIONPROPERTYEDITOR_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QIntValidator>

class AnimationPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    AnimationPropertyEditor();

    void setAnimation(QPropertyAnimation *anim);

public slots:
    void beginChanged(int);
    void durationChanged(int);
    void fromChanged(int);
    void toChanged(int);

signals:
    void dataChanged();

private:
    QPropertyAnimation *m_animation;
    QLineEdit *m_property;
    QSpinBox *m_begin;
    QSpinBox *m_duration;
    QSpinBox *m_from;
    QSpinBox *m_to;
    QComboBox *m_easing;
};

#endif // ANIMATIONPROPERTYEDITOR_H
