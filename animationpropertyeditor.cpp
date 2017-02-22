#include "animationpropertyeditor.h"
#include "expander.h"

#include <QGridLayout>
#include <limits.h>

AnimationPropertyEditor::AnimationPropertyEditor()
{
    QVBoxLayout *vbox = new QVBoxLayout();
    Expander *exp = new Expander("Animation");
    m_property = new QLineEdit();
    m_begin = new QSpinBox();
    m_duration = new QSpinBox();
    m_from = new QSpinBox();
    m_to = new QSpinBox();
    m_easing = new QComboBox();
    m_begin->setMinimum(0);
    m_begin->setMaximum(900000);
    m_duration->setMinimum(0);
    m_duration->setMaximum(900000);
    m_property->setEnabled(false);
    m_easing->addItem("Linear");
    m_easing->addItem("InQuad");
    m_easing->addItem("OutQuad");
    m_easing->addItem("InOutQuad");
    m_easing->addItem("OutInQuad");
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(new QLabel("Property"), 0, 0);
    layout->addWidget(m_property, 0, 1, 1, 4);
    layout->addWidget(new QLabel("Time"), 1, 0);
    layout->addWidget(new QLabel("Value"), 2, 0);
    layout->addWidget(new QLabel("Begin"), 1, 1);
    layout->addWidget(m_begin, 1, 2);
    layout->addWidget(new QLabel("Duration"), 1, 3);
    layout->addWidget(m_duration, 1, 4);
    layout->addWidget(new QLabel("From"), 2, 1);
    layout->addWidget(m_from, 2, 2);
    layout->addWidget(new QLabel("To"), 2, 3);
    layout->addWidget(m_to, 2, 4);
    layout->addWidget(new QLabel("Easing"), 3, 0);
    layout->addWidget(m_easing, 3, 1, 1, 4);
    exp->addLayout(layout);
    vbox->addWidget(exp);
    vbox->addStretch();
    this->setLayout(vbox);

    connect(m_begin, SIGNAL(valueChanged(int)), this, SLOT(beginChanged(int)));
    connect(m_duration, SIGNAL(valueChanged(int)), this, SLOT(durationChanged(int)));
    connect(m_from, SIGNAL(valueChanged(int)), this, SLOT(fromChanged(int)));
    connect(m_to, SIGNAL(valueChanged(int)), this, SLOT(toChanged(int)));
}

void AnimationPropertyEditor::setAnimation(QPropertyAnimation *anim)
{
    m_animation = anim;
    m_property->setText(anim->propertyName());
    m_begin->setValue(anim->property("begin").toInt());
    m_duration->setValue(anim->duration());
    m_from->setMinimum(anim->property("min").toInt());
    m_from->setMaximum(anim->property("max").toInt());
    m_to->setMinimum(anim->property("min").toInt());
    m_to->setMaximum(anim->property("max").toInt());
    m_from->setValue(anim->startValue().toInt());
    m_to->setValue(anim->endValue().toInt());
}

void AnimationPropertyEditor::beginChanged(int newValue)
{
    m_animation->setProperty("begin", newValue);
    emit dataChanged();
}

void AnimationPropertyEditor::durationChanged(int newValue)
{
    m_animation->setDuration(newValue);
    emit dataChanged();
}

void AnimationPropertyEditor::fromChanged(int newValue)
{
    m_animation->setStartValue(newValue);
    emit dataChanged();
}

void AnimationPropertyEditor::toChanged(int newValue)
{
    m_animation->setEndValue(newValue);
    emit dataChanged();
}
