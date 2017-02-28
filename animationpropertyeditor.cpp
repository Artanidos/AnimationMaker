#include "animationpropertyeditor.h"
#include "expander.h"

#include <QGridLayout>
#include <QTest>
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
    m_easing->addItem("Linear", QVariant(QEasingCurve::Linear));
    m_easing->addItem("InQuad", QVariant(QEasingCurve::InQuad));
    m_easing->addItem("OutQuad", QVariant(QEasingCurve::OutQuad));
    m_easing->addItem("InOutQuad", QVariant(QEasingCurve::InOutQuad));
    m_easing->addItem("OutInQuad", QVariant(QEasingCurve::OutInQuad));
    m_easing->addItem("InCubic", QVariant(QEasingCurve::InCubic));
    m_easing->addItem("OutCubic", QVariant(QEasingCurve::OutCubic));
    m_easing->addItem("InOutCubic", QVariant(QEasingCurve::InOutCubic));
    m_easing->addItem("OutInCubic", QVariant(QEasingCurve::OutInCubic));
    m_easing->addItem("InQuart", QVariant(QEasingCurve::InQuart));
    m_easing->addItem("OutQuart", QVariant(QEasingCurve::OutQuart));
    m_easing->addItem("InOutQuart", QVariant(QEasingCurve::InOutQuart));
    m_easing->addItem("OutInQuart", QVariant(QEasingCurve::OutInQuart));
    m_easing->addItem("InQuint", QVariant(QEasingCurve::InQuint));
    m_easing->addItem("OutQuint", QVariant(QEasingCurve::OutQuint));
    m_easing->addItem("InOutQuint", QVariant(QEasingCurve::InOutQuint));
    m_easing->addItem("OutInQuint", QVariant(QEasingCurve::OutInQuint));
    m_easing->addItem("InSine", QVariant(QEasingCurve::InSine));
    m_easing->addItem("OutSine", QVariant(QEasingCurve::OutSine));
    m_easing->addItem("InOutSine", QVariant(QEasingCurve::InOutSine));
    m_easing->addItem("OutInSine", QVariant(QEasingCurve::OutInSine));
    m_easing->addItem("InExpo", QVariant(QEasingCurve::InExpo));
    m_easing->addItem("OutExpo", QVariant(QEasingCurve::OutExpo));
    m_easing->addItem("InOutExpo", QVariant(QEasingCurve::InOutExpo));
    m_easing->addItem("OutInExpo", QVariant(QEasingCurve::OutInExpo));
    m_easing->addItem("InCirc", QVariant(QEasingCurve::InCirc));
    m_easing->addItem("OutCirc", QVariant(QEasingCurve::OutCirc));
    m_easing->addItem("InOutCirc", QVariant(QEasingCurve::InOutCirc));
    m_easing->addItem("OutInCirc", QVariant(QEasingCurve::OutInCirc));
    m_easing->addItem("InElastic", QVariant(QEasingCurve::InElastic));
    m_easing->addItem("OutElastic", QVariant(QEasingCurve::OutElastic));
    m_easing->addItem("InOutElastic", QVariant(QEasingCurve::InOutElastic));
    m_easing->addItem("OutInElastic", QVariant(QEasingCurve::OutInElastic));
    m_easing->addItem("InBack", QVariant(QEasingCurve::InBack));
    m_easing->addItem("OutBack", QVariant(QEasingCurve::OutBack));
    m_easing->addItem("InOutBack", QVariant(QEasingCurve::InOutBack));
    m_easing->addItem("OutInBack", QVariant(QEasingCurve::OutInBack));
    m_easing->addItem("InBounce", QVariant(QEasingCurve::InBounce));
    m_easing->addItem("OutBounce", QVariant(QEasingCurve::OutBounce));
    m_easing->addItem("InOutBounce", QVariant(QEasingCurve::InOutBounce));
    m_easing->addItem("OutInBounce", QVariant(QEasingCurve::OutInBounce));

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
    connect(m_easing, SIGNAL(currentIndexChanged(int)), this, SLOT(easingChanged(int)));
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

    int easing = (int)anim->easingCurve().type();
    int index = m_easing->findData(QVariant(easing));
    if(index < 0)
        index = 0;
    m_easing->setCurrentIndex(index);
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

void AnimationPropertyEditor::easingChanged(int newValue)
{
    m_animation->setEasingCurve((QEasingCurve::Type)m_easing->itemData(newValue).toInt());
}
