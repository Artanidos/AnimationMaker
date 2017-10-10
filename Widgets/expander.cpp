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

#include "expander.h"

#include <QVBoxLayout>
#include <QLabel>


Expander::Expander(QString header)
{
    m_isExpanded = true;
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(64, 66, 68));
    setAutoFillBackground(true);
    setPalette(pal);
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
    QPalette cpal = m_content->palette();
    cpal.setColor(QPalette::Background, QColor(76, 78, 80));
    m_content->setPalette(cpal);
    m_content->setAutoFillBackground(true);
    vbox->addWidget(m_content);
    this->setLayout(vbox);
    m_contentLayout = new QVBoxLayout;
    m_content->setLayout(m_contentLayout);

    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
}

void Expander::buttonClicked()
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
    m_contentLayout->addLayout(l);
 }
