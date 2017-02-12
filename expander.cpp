#include "expander.h"

#include <QVBoxLayout>
#include <QLabel>


Expander::Expander(QString header)
{
    m_isExpanded = true;
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(64, 66, 68));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();
    m_button = new QToolButton();
    m_button->setStyleSheet("QToolButton { border: none; }");
    m_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_button->setArrowType(Qt::ArrowType::DownArrow);
    m_button->setText(header);
    m_button->setCheckable(true);
    m_button->setChecked(false);

    QFont headerFont = m_button->font();
    headerFont.setBold(true);
    m_button->setFont(headerFont);

    hbox->addWidget(m_button);
    hbox->addStretch();
    vbox->addLayout(hbox);
    m_content = new QWidget();
    m_content->setStyleSheet("background-color: #4c4e50");
    vbox->addWidget(m_content);
    this->setLayout(vbox);

    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
}

void Expander::buttonClicked(bool)
{
    if(m_isExpanded)
        m_isExpanded = false;
    else
        m_isExpanded = true;
    m_content->setHidden(!m_isExpanded);
    m_button->setArrowType(m_isExpanded ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
}

 void Expander::addLayout(QLayout *l)
 {
    m_content->setLayout(l);
 }
