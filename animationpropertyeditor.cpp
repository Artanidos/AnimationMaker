#include "animationpropertyeditor.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QIntValidator>

AnimationPropertyEditor::AnimationPropertyEditor()
{
    QVBoxLayout *vbox = new QVBoxLayout();
    QLabel *label = new QLabel("Animation");
    QFont font = label->font();
    font.setBold(true);
    label->setFont(font);

    //new QDoubleValidator(0, 100, 2, this
    QLineEdit *m_property = new QLineEdit();
    QSpinBox *m_begin = new QSpinBox();
    QSpinBox *m_duration = new QSpinBox();
    QSpinBox *m_from = new QSpinBox();
    QSpinBox *m_to = new QSpinBox();
    QComboBox *m_easing = new QComboBox();
    m_begin->setValue(0);
    m_begin->setMinimum(0);
    m_duration->setValue(0);
    m_duration->setMinimum(0);
    m_from->setMinimum(0);
    m_from->setMaximum(100);
    m_from->setValue(100);
    m_to->setMinimum(0);
    m_to->setMaximum(100);
    m_to->setValue(100);
    m_property->setText("Opacity");
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
    vbox->addWidget(label);
    vbox->addLayout(layout);
    vbox->addStretch();
    this->setLayout(vbox);
}
