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
    m_rectangle = NULL;
    m_ellipse = NULL;

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
    addXKeyframe->setToolTip("Add keyframe for Left");
    QPushButton *addYKeyframe = new QPushButton();
    addYKeyframe->setStyleSheet(buttonStyle);
    addYKeyframe->setMaximumWidth(9);
    addYKeyframe->setToolTip("Add keyframe for Top");
    QPushButton *addWidthKeyframe = new QPushButton();
    addWidthKeyframe->setStyleSheet(buttonStyle);
    addWidthKeyframe->setMaximumWidth(9);
    addWidthKeyframe->setToolTip("Add keyframe for Width");
    QPushButton *addHeightKeyframe = new QPushButton();
    addHeightKeyframe->setStyleSheet(buttonStyle);
    addHeightKeyframe->setMaximumWidth(9);
    addHeightKeyframe->setToolTip("Add keyframe  for Height");
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
    m_hueTextColorSlider = new QSlider();
    m_textcolorRect = new ColorRect();
    m_hueTextColorSlider->setMinimum(0);
    m_hueTextColorSlider->setMaximum(100);
    m_hueTextColorSlider->setOrientation(Qt::Vertical);
    m_hueTextColorSlider->setMaximumHeight(100);
    QGridLayout *layoutTextcolor = new QGridLayout();
    QLabel *labelColor = new QLabel("Color");
    m_textcolor = new QLineEdit();
    layoutTextcolor->addWidget(labelColor, 0, 0);
    layoutTextcolor->addWidget(m_textcolorRect, 0, 1);
    layoutTextcolor->addWidget(m_textcolor, 0, 2);
    layoutTextcolor->addWidget(m_textcolorpicker, 1, 0);
    layoutTextcolor->addWidget(m_hueTextColorSlider, 1, 1);
    expTextcolor->addLayout(layoutTextcolor);
    vbox->addWidget(expTextcolor);

    m_colorPicker = new ColorPicker();
    m_colorPicker->setVisible(false);
    m_bordercolorPicker = new ColorPicker();
    m_bordercolorPicker->setVisible(false);
    m_hueColorSlider = new QSlider();
    m_hueColorSlider->setVisible(false);
    m_hueColorSlider->setMinimum(0);
    m_hueColorSlider->setMaximum(100);
    m_hueColorSlider->setOrientation(Qt::Vertical);
    m_hueColorSlider->setMaximumHeight(100);

    m_hueBordercolorSlider = new QSlider();
    m_hueBordercolorSlider->setVisible(false);
    m_hueBordercolorSlider->setMinimum(0);
    m_hueBordercolorSlider->setMaximum(100);
    m_hueBordercolorSlider->setOrientation(Qt::Vertical);
    m_hueBordercolorSlider->setMaximumHeight(100);

    expColor = new Expander("Color");
    expColor->setVisible(false);
    QGridLayout *layoutColor = new QGridLayout();
    QLabel *labelBrush = new QLabel("Brush");
    QLabel *labelBorder = new QLabel("Border");
    m_colorRect = new ColorRect();
    m_borderColorRect = new ColorRect();
    m_brushcolor = new QLineEdit();
    m_pencolor = new QLineEdit();
    layoutColor->addWidget(labelBrush, 0, 0);
    layoutColor->addWidget(m_colorRect, 0, 1);
    layoutColor->addWidget(m_brushcolor, 0, 2);
    layoutColor->addWidget(m_colorPicker, 1, 0);
    layoutColor->addWidget(m_hueColorSlider, 1, 1);
    layoutColor->addWidget(labelBorder, 2, 0);
    layoutColor->addWidget(m_borderColorRect, 2, 1);
    layoutColor->addWidget(m_pencolor, 2, 2);
    layoutColor->addWidget(m_bordercolorPicker, 3, 0);
    layoutColor->addWidget(m_hueBordercolorSlider, 3, 1);
    expColor->addLayout(layoutColor);
    vbox->addWidget(expColor);

    Expander *expOpacity = new Expander("Visibility");
    QGridLayout *layoutOpacity = new QGridLayout();
    QPushButton *addOpacityKeyframe = new QPushButton();
    addOpacityKeyframe->setStyleSheet(buttonStyle);
    addOpacityKeyframe->setMaximumWidth(9);
    addOpacityKeyframe->setToolTip("Add keyframe for Opacity");
    QLabel *labelOpacity = new QLabel("Opacity");
    m_opacity = new QSlider(Qt::Horizontal);
    m_opacity->setMinimum(0);
    m_opacity->setMaximum(100);
    m_opacityText = new QSpinBox();
    m_opacityText->setMinimum(0);
    m_opacityText->setMaximum(100);
    layoutOpacity->addWidget(labelOpacity, 0, 0);
    layoutOpacity->addWidget(m_opacity, 0, 1);
    layoutOpacity->addWidget(addOpacityKeyframe, 0, 2);
    layoutOpacity->addWidget(m_opacityText, 0, 3);
    expOpacity->addLayout(layoutOpacity);
    vbox->addWidget(expOpacity);

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
    connect(m_hueColorSlider, SIGNAL(valueChanged(int)), this, SLOT(hueChanged(int)));
    connect(m_colorPicker, SIGNAL(colorChanged(QColor)), this, SLOT(colorChanged(QColor)));
    connect(m_hueBordercolorSlider, SIGNAL(valueChanged(int)), this, SLOT(hueBorderChanged(int)));
    connect(m_bordercolorPicker, SIGNAL(colorChanged(QColor)), this, SLOT(bordercolorChanged(QColor)));
    connect(m_hueTextColorSlider, SIGNAL(valueChanged(int)), this, SLOT(hueTextcolorChanged(int)));
    connect(m_textcolorpicker, SIGNAL(colorChanged(QColor)), this, SLOT(textcolorChanged(QColor)));
    connect(addXKeyframe, SIGNAL(clicked(bool)), this, SLOT(addLeftKeyFrame()));
    connect(addYKeyframe, SIGNAL(clicked(bool)), this, SLOT(addTopKeyFrame()));
    connect(addWidthKeyframe, SIGNAL(clicked(bool)), this, SLOT(addWidthKeyFrame()));
    connect(addHeightKeyframe, SIGNAL(clicked(bool)), this, SLOT(addHeightKeyFrame()));
    connect(m_opacity, SIGNAL(valueChanged(int)), this, SLOT(opacityChanged(int)));
    connect(m_opacityText, SIGNAL(valueChanged(int)), this, SLOT(opacityTextChanged(int)));
    connect(addOpacityKeyframe, SIGNAL(clicked(bool)), this, SLOT(addOpacityKeyFrame()));
    connect(m_colorRect, SIGNAL(mouseClicked()), this, SLOT(colorRectClicked()));
    connect(m_borderColorRect, SIGNAL(mouseClicked()), this, SLOT(borderColorRectClicked()));
    connect(m_textcolorRect, SIGNAL(mouseClicked()), this, SLOT(textColorRectClicked()));
}

void ItemPropertyEditor::addLeftKeyFrame()
{
    emit addKeyFrame(m_item, "left", m_x->value());
}

void ItemPropertyEditor::addTopKeyFrame()
{
    emit addKeyFrame(m_item, "top", m_y->value());
}

void ItemPropertyEditor::addWidthKeyFrame()
{
    emit addKeyFrame(m_item, "width", m_width->value());
}

void ItemPropertyEditor::addHeightKeyFrame()
{
    emit addKeyFrame(m_item, "height", m_height->value());
}

void ItemPropertyEditor::addOpacityKeyFrame()
{
    emit addKeyFrame(m_item, "opacity", (qreal)m_opacity->value() / 100);
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
    m_opacity->setValue(item->opacity() * 100);
    m_opacityText->setValue(item->opacity() * 100);

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

    connect(item, SIGNAL(sizeChanged(qreal,qreal)), this, SLOT(itemSizeChanged(qreal, qreal)));
    connect(item, SIGNAL(positionChanged(qreal,qreal)), this, SLOT(itemPositionChanged(qreal, qreal)));
}

void ItemPropertyEditor::itemSizeChanged(qreal width, qreal height)
{
    m_width->setValue(width);
    m_height->setValue(height);
}

void ItemPropertyEditor::itemPositionChanged(qreal x, qreal y)
{
    m_x->setValue(x);
    m_y->setValue(y);
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
    m_item->adjustKeyframes("width", QVariant(value));
}

void ItemPropertyEditor::heightChanged(int value)
{
    m_item->setHeight(value);
    m_item->adjustKeyframes("height", QVariant(value));
}

void ItemPropertyEditor::textChanged(QString value)
{
    m_textitem->setText(value);
}

void ItemPropertyEditor::textcolorChanged(QString value)
{
    m_textitem->setTextcolor(QColor(value));
    m_textcolorRect->setColor(QColor(value));
}

void ItemPropertyEditor::textcolorChanged(QColor value)
{
    m_textitem->setTextcolor(value);
    m_textcolor->setText(value.name());
    m_textcolorRect->setColor(value);
}

void ItemPropertyEditor::colorChanged(QString value)
{
    if(m_rectangle)
        m_rectangle->setBrush(QBrush(QColor(value)));

    if(m_ellipse)
        m_ellipse->setBrush(QBrush(QColor(value)));

    m_colorRect->setColor(QColor(value));
}

void ItemPropertyEditor::colorChanged(QColor value)
{
    if(m_rectangle)
        m_rectangle->setBrush(QBrush(value));

    if(m_ellipse)
        m_ellipse->setBrush(QBrush(value));
    m_brushcolor->setText(value.name());
    m_colorRect->setColor(value);
}

void ItemPropertyEditor::borderColorChanged(QString value)
{
    if(m_rectangle)
        m_rectangle->setPen(QPen(QColor(value)));

    if(m_ellipse)
        m_ellipse->setPen(QPen(QColor(value)));
    m_borderColorRect->setColor(QColor(value));
}

void ItemPropertyEditor::bordercolorChanged(QColor value)
{
    if(m_rectangle)
        m_rectangle->setPen(QPen(value));

    if(m_ellipse)
        m_ellipse->setPen(QPen(value));
    m_pencolor->setText(value.name());
    m_borderColorRect->setColor(value);
}

void ItemPropertyEditor::hueChanged(int value)
{
    m_colorPicker->setHue((qreal)value / 100.0);
}

void ItemPropertyEditor::hueBorderChanged(int value)
{
    m_bordercolorPicker->setHue((qreal)value / 100.0);
}

void ItemPropertyEditor::hueTextcolorChanged(int value)
{
    m_textcolorpicker->setHue((qreal)value / 100.0);
}

void ItemPropertyEditor::opacityChanged(int value)
{
    m_item->setOpacity((qreal)value / 100);
    m_opacityText->setValue(value);
    m_item->adjustKeyframes("opacity", QVariant((qreal)value / 100));
}

void ItemPropertyEditor::opacityTextChanged(int value)
{
    m_item->setOpacity((qreal)value / 100);
    m_opacity->setValue(value);
    m_item->adjustKeyframes("opacity", QVariant((qreal)value / 100));
}


void ItemPropertyEditor::colorRectClicked()
{
    bool visible = m_colorPicker->isVisible();

    m_colorPicker->setVisible(!visible);
    m_hueColorSlider->setVisible(!visible);

    if(!visible)
    {
        m_bordercolorPicker->setVisible(visible);
        m_hueBordercolorSlider->setVisible(visible);
    }
}

void ItemPropertyEditor::borderColorRectClicked()
{
    bool visible = m_bordercolorPicker->isVisible();

    m_bordercolorPicker->setVisible(!visible);
    m_hueBordercolorSlider->setVisible(!visible);

    if(!visible)
    {
        m_colorPicker->setVisible(visible);
        m_hueColorSlider->setVisible(visible);
    }
}

void ItemPropertyEditor::textColorRectClicked()
{
    bool visible = m_textcolorpicker->isVisible();

    m_textcolorpicker->setVisible(!visible);
    m_hueTextColorSlider->setVisible(!visible);
}
