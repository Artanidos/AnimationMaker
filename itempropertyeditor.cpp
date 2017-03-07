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

#include "itempropertyeditor.h"
#include "animationscene.h"
#include <QPushButton>

ItemPropertyEditor::ItemPropertyEditor()
{
    QString buttonStyle = "QPushButton{border: none;image:url(:/images/raute.png)} QPushButton:hover{border: none;image:url(:/images/raute-hover.png)} QToolTip{background:#f5f0eb;}";
    QVBoxLayout *vbox = new QVBoxLayout();
    Expander *expTyp = new Expander("Typ");
    QGridLayout *layoutTyp = new QGridLayout();
    QLabel *labelTyp = new QLabel("Typ");
    m_typ = new QLabel("Unknown");
    QLabel *labelId = new QLabel("Id");
    m_id = new QLineEdit();
    layoutTyp->addWidget(labelTyp, 0, 0);
    layoutTyp->addWidget(m_typ, 0, 1);
    layoutTyp->addWidget(labelId, 1, 0);
    layoutTyp->addWidget(m_id, 1, 1);
    expTyp->addLayout(layoutTyp);
    vbox->addWidget(expTyp);

    Expander *expGeo = new Expander("Geometrie");
    QGridLayout *layoutGeo = new QGridLayout();
    QLabel *labelPosition = new QLabel("Position");
    QLabel *labelSize = new QLabel("Size");
    QLabel *labelX = new QLabel("X");
    QLabel *labelY = new QLabel("Y");
    QLabel *labelWidth = new QLabel("W");
    QLabel *labelHeight = new QLabel("H");
    labelX->setFixedWidth(15);
    labelY->setFixedWidth(15);
    labelWidth->setFixedWidth(15);
    labelHeight->setFixedWidth(15);
    m_x = new QSpinBox();
    m_y = new QSpinBox();
    m_width = new QSpinBox();
    m_height = new QSpinBox();
    m_x->setMinimum(-10000);
    m_x->setMaximum(10000);
    m_y->setMinimum(-10000);
    m_y->setMaximum(10000);
    m_width->setMaximum(10000);
    m_height->setMaximum(10000);
    QPushButton *addXKeyframe = new QPushButton();
    addXKeyframe->setStyleSheet(buttonStyle);
    addXKeyframe->setMaximumWidth(9);
    addXKeyframe->setToolTip("Add keyframe Left");
    QPushButton *addYKeyframe = new QPushButton();
    addYKeyframe->setStyleSheet(buttonStyle);
    addYKeyframe->setMaximumWidth(9);
    addYKeyframe->setToolTip("Add keyframe Top");
    QPushButton *addWidthKeyframe = new QPushButton();
    addWidthKeyframe->setStyleSheet(buttonStyle);
    addWidthKeyframe->setMaximumWidth(9);
    addWidthKeyframe->setToolTip("Add keyframe Width");
    QPushButton *addHeightKeyframe = new QPushButton();
    addHeightKeyframe->setStyleSheet(buttonStyle);
    addHeightKeyframe->setMaximumWidth(9);
    addHeightKeyframe->setToolTip("Add keyframe Height");
    layoutGeo->addWidget(labelPosition, 0, 0);
    layoutGeo->addWidget(labelX, 0, 1);
    layoutGeo->addWidget(addXKeyframe, 0, 2);
    layoutGeo->addWidget(m_x, 0, 3);
    layoutGeo->addWidget(labelY, 0, 4);
    layoutGeo->addWidget(addYKeyframe, 0, 5);
    layoutGeo->addWidget(m_y, 0, 6);

    layoutGeo->addWidget(labelSize, 1, 0);
    layoutGeo->addWidget(labelWidth, 1, 1);
    layoutGeo->addWidget(addWidthKeyframe, 1, 2);
    layoutGeo->addWidget(m_width, 1, 3);
    layoutGeo->addWidget(labelHeight, 1, 4);
    layoutGeo->addWidget(addHeightKeyframe, 1, 5);
    layoutGeo->addWidget(m_height, 1, 6);
    expGeo->addLayout(layoutGeo);
    vbox->addWidget(expGeo);

    expText = new Expander("Text");
    expText->setVisible(false);
    QGridLayout *layoutText = new QGridLayout();
    QLabel *labelText = new QLabel("Text");
    m_text = new QLineEdit();
    layoutText->addWidget(labelText, 0, 0);
    layoutText->addWidget(m_text, 0, 1);
    expText->addLayout(layoutText);
    vbox->addWidget(expText);

    expTextcolor = new Expander("Textcolor");
    m_textcolorpicker = new ColorPicker();
    expTextcolor->setVisible(false);
    QSlider *hueTextColorSlider = new QSlider();
    hueTextColorSlider->setMinimum(0);
    hueTextColorSlider->setMaximum(100);
    hueTextColorSlider->setOrientation(Qt::Vertical);
    hueTextColorSlider->setMaximumHeight(100);
    QGridLayout *layoutTextcolor = new QGridLayout();
    QLabel *labelColor = new QLabel("Color");
    m_textcolor = new QLineEdit();
    layoutTextcolor->addWidget(labelColor, 0, 0);
    layoutTextcolor->addWidget(m_textcolor, 0, 1);
    layoutTextcolor->addWidget(m_textcolorpicker, 1, 0);
    layoutTextcolor->addWidget(hueTextColorSlider, 1, 1);
    expTextcolor->addLayout(layoutTextcolor);
    vbox->addWidget(expTextcolor);

    m_colorpicker = new ColorPicker();
    QSlider *hueColorSlider = new QSlider();
    hueColorSlider->setMinimum(0);
    hueColorSlider->setMaximum(100);
    hueColorSlider->setOrientation(Qt::Vertical);
    hueColorSlider->setMaximumHeight(100);

    expColor = new Expander("Color");
    expColor->setVisible(false);
    QGridLayout *layoutColor = new QGridLayout();
    QLabel *labelBrush = new QLabel("Brush");
    QLabel *labelBorder = new QLabel("Border");
    m_brushcolor = new QLineEdit();
    m_pencolor = new QLineEdit();
    layoutColor->addWidget(labelBrush, 0, 0);
    layoutColor->addWidget(m_brushcolor, 0, 1);
    layoutColor->addWidget(m_colorpicker, 1, 0);
    layoutColor->addWidget(hueColorSlider, 1, 1);
    layoutColor->addWidget(labelBorder, 2, 0);
    layoutColor->addWidget(m_pencolor, 2, 1);
    expColor->addLayout(layoutColor);
    vbox->addWidget(expColor);

    vbox->addStretch();
    this->setLayout(vbox);


    connect(m_id, SIGNAL(textChanged(QString)), this, SLOT(idChanged(QString)));
    connect(m_x, SIGNAL(valueChanged(int)), this, SLOT(xChanged(int)));
    connect(m_y, SIGNAL(valueChanged(int)), this, SLOT(yChanged(int)));
    connect(m_width, SIGNAL(valueChanged(int)), this, SLOT(widthChanged(int)));
    connect(m_height, SIGNAL(valueChanged(int)), this, SLOT(heightChanged(int)));
    connect(m_text, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(m_textcolor, SIGNAL(textChanged(QString)), this, SLOT(textcolorChanged(QString)));
    connect(m_brushcolor, SIGNAL(textChanged(QString)), this, SLOT(colorChanged(QString)));
    connect(m_pencolor, SIGNAL(textChanged(QString)), this, SLOT(borderColorChanged(QString)));
    connect(hueColorSlider, SIGNAL(valueChanged(int)), this, SLOT(hueChanged(int)));
    connect(m_colorpicker, SIGNAL(colorChanged(QColor)), this, SLOT(colorChanged(QColor)));
    connect(hueTextColorSlider, SIGNAL(valueChanged(int)), this, SLOT(hueTextcolorChanged(int)));
    connect(m_textcolorpicker, SIGNAL(colorChanged(QColor)), this, SLOT(textcolorChanged(QColor)));
    connect(addXKeyframe, SIGNAL(clicked(bool)), this, SLOT(addLeftKeyFrame()));
}

void ItemPropertyEditor::addLeftKeyFrame()
{

}

void ItemPropertyEditor::setItem(ResizeableItem *item)
{
    m_item = item;
    expColor->setVisible(false);
    m_x->setValue(m_item->x());
    m_y->setValue(m_item->y());
    m_width->setValue(m_item->rect().width());
    m_height->setValue(m_item->rect().height());
    m_typ->setText(getItemTypeName(m_item));
    m_id->setText(item->id());

    m_textitem = dynamic_cast<Text*>(item);
    if(m_textitem)
    {
        m_text->setText(m_textitem->text());
        m_textcolor->setText(m_textitem->textcolor().name());
    }

    m_rectangle = dynamic_cast<Rectangle*>(item);
    if(m_rectangle)
    {
        m_brushcolor->setText(m_rectangle->brush().color().name());
        m_pencolor->setText(m_rectangle->pen().color().name());
        expColor->setVisible(true);
    }

    m_ellipse = dynamic_cast<Ellipse*>(item);
    if(m_ellipse)
    {
        m_brushcolor->setText(m_ellipse->brush().color().name());
        m_pencolor->setText(m_ellipse->pen().color().name());
        expColor->setVisible(true);
    }
    expText->setVisible(m_textitem);
    expTextcolor->setVisible(m_textitem);
}

void ItemPropertyEditor::idChanged(QString value)
{
    m_item->setId(value);
}

void ItemPropertyEditor::xChanged(int value)
{
    m_item->setX(value);
}

void ItemPropertyEditor::yChanged(int value)
{
    m_item->setY(value);
}

void ItemPropertyEditor::widthChanged(int value)
{
    m_item->setWidth(value);
}

void ItemPropertyEditor::heightChanged(int value)
{
    m_item->setHeight(value);
}

void ItemPropertyEditor::textChanged(QString value)
{
    m_textitem->setText(value);
}

void ItemPropertyEditor::textcolorChanged(QString value)
{
    m_textitem->setTextcolor(QColor(value));
}

void ItemPropertyEditor::textcolorChanged(QColor value)
{
    m_textitem->setTextcolor(value);
    m_textcolor->setText(value.name());
}

void ItemPropertyEditor::colorChanged(QString value)
{
    if(m_rectangle)
        m_rectangle->setBrush(QBrush(QColor(value)));

    if(m_ellipse)
        m_ellipse->setBrush(QBrush(QColor(value)));
}

void ItemPropertyEditor::colorChanged(QColor value)
{
    if(m_rectangle)
        m_rectangle->setBrush(QBrush(value));

    if(m_ellipse)
        m_ellipse->setBrush(QBrush(value));
    m_brushcolor->setText(value.name());
}

void ItemPropertyEditor::borderColorChanged(QString value)
{
    if(m_rectangle)
        m_rectangle->setPen(QPen(QColor(value)));

    if(m_ellipse)
        m_ellipse->setPen(QPen(QColor(value)));
}

void ItemPropertyEditor::hueChanged(int value)
{
    m_colorpicker->setHue((qreal)value / 100.0);
}

void ItemPropertyEditor::hueTextcolorChanged(int value)
{
    m_textcolorpicker->setHue((qreal)value / 100.0);
}
