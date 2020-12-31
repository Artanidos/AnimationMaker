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

#include "svgattributeeditor.h"

#include <QGridLayout>
#include <QLabel>

#include "flatbutton.h"

SvgAttributeEditor::SvgAttributeEditor()
{
    QGridLayout *layout = new QGridLayout();
    m_element = new QLineEdit;
    m_attribute = new QLineEdit;
    m_value = new QLineEdit;

    FlatButton *addAttributeKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addAttributeKeyframe->setMaximumWidth(9);
    addAttributeKeyframe->setToolTip("Add keyframe for svg attribute");

    FlatButton *minus = new FlatButton(":/images/minus_normal.png", ":/images/minus_hover.png");
    minus->setToolTip("Delete attribute");

    layout->addWidget(new QLabel("Id"), 0, 0);
    layout->addWidget(m_element, 0, 1);
    layout->addWidget(new QLabel("Att"), 0, 2);
    layout->addWidget(m_attribute, 0, 3);
    layout->addWidget(new QLabel("Val"), 0, 4);
    layout->addWidget(addAttributeKeyframe, 0, 5);
    layout->addWidget(m_value, 0, 6);
    layout->addWidget(minus, 0, 7);
    setLayout(layout);

    connect(m_element, SIGNAL(editingFinished()), this, SLOT(attributeNameChanged()));
    connect(m_attribute, SIGNAL(editingFinished()), this, SLOT(attributeNameChanged()));
    connect(m_value, SIGNAL(editingFinished()), this, SLOT(valueChanged()));
    connect(minus, SIGNAL(clicked()), this, SLOT(minusClicked()));
    connect(addAttributeKeyframe, SIGNAL(clicked()), this, SLOT(addKeyframeClicked()));
}

void SvgAttributeEditor::setAttributeName(QString name)
{
    int dot = name.indexOf(".");
    if(dot > 0)
    {
        QString id = name.mid(0, dot);
        QString attribute = name.mid(dot + 1);
        m_element->setText(id);
        m_attribute->setText(attribute);
    }
}

void SvgAttributeEditor::attributeNameChanged()
{
    if(isValid())
    {
        emit attributeNameChanged(m_attributeName, attributeName());
        m_attributeName = attributeName();
    }
}

void SvgAttributeEditor::valueChanged()
{
    if(isValid())
        emit attributeValueChanged(attributeName(), m_value->text());
}

void SvgAttributeEditor::minusClicked()
{
    emit removeClicked(this);
}

void SvgAttributeEditor::addKeyframeClicked()
{
    emit addKeyframeClicked(this);
}
