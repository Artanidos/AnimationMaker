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
#include "commands.h"
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
    m_textcolorpicker->setVisible(false);
    expTextcolor->setVisible(false);
    m_hueTextColorSlider = new QSlider();
    m_hueTextColorSlider->setVisible(false);
    m_textcolorRect = new ColorRect();
    m_hueTextColorSlider->setMinimum(0);
    m_hueTextColorSlider->setMaximum(100);
    m_hueTextColorSlider->setOrientation(Qt::Vertical);
    m_hueTextColorSlider->setMaximumHeight(100);

    m_hueText = new QSpinBox();
    m_saturationText = new QSpinBox();
    m_lightnessText = new QSpinBox();
    m_labelHueText = new QLabel("H");
    m_labelSaturationText = new QLabel("S");
    m_labelLightnessText = new QLabel("L");
    m_redText = new QSpinBox();
    m_greenText = new QSpinBox();
    m_blueText = new QSpinBox();
    m_labelRedText = new QLabel("R");
    m_labelGreenText = new QLabel("G");
    m_labelBlueText = new QLabel("B");
    m_hueText->setVisible(false);
    m_saturationText->setVisible(false);
    m_lightnessText->setVisible(false);
    m_redText->setVisible(false);
    m_greenText->setVisible(false);
    m_blueText->setVisible(false);
    m_labelHueText->setVisible(false);
    m_labelSaturationText->setVisible(false);
    m_labelLightnessText->setVisible(false);
    m_labelRedText->setVisible(false);
    m_labelGreenText->setVisible(false);
    m_labelBlueText->setVisible(false);
    m_redText->setMinimum(0);
    m_redText->setMaximum(255);
    m_greenText->setMinimum(0);
    m_greenText->setMaximum(255);
    m_blueText->setMinimum(0);
    m_blueText->setMaximum(255);
    m_hueText->setMinimum(0);
    m_hueText->setMaximum(100);
    m_saturationText->setMinimum(0);
    m_saturationText->setMaximum(100);
    m_lightnessText->setMinimum(0);
    m_lightnessText->setMaximum(100);
    m_labelHueText->setFixedWidth(15);
    m_labelSaturationText->setFixedWidth(15);
    m_labelLightnessText->setFixedWidth(15);
    m_labelRedText->setFixedWidth(15);
    m_labelGreenText->setFixedWidth(15);
    m_labelBlueText->setFixedWidth(15);

    QGridLayout *layoutTextcolor = new QGridLayout();
    QLabel *labelColor = new QLabel("Color");
    m_textcolor = new QLineEdit();
    layoutTextcolor->addWidget(labelColor, 0, 0);
    layoutTextcolor->addWidget(m_textcolorRect, 0, 1);
    layoutTextcolor->addWidget(m_textcolor, 0, 2, 1, 4);
    layoutTextcolor->addWidget(m_textcolorpicker, 1, 0, 3, 1);
    layoutTextcolor->addWidget(m_hueTextColorSlider, 1, 1, 3, 1);
    layoutTextcolor->addWidget(m_labelHueText, 1, 2);
    layoutTextcolor->addWidget(m_hueText, 1, 3);
    layoutTextcolor->addWidget(m_labelSaturationText, 2, 2);
    layoutTextcolor->addWidget(m_saturationText, 2, 3);
    layoutTextcolor->addWidget(m_labelLightnessText, 3, 2);
    layoutTextcolor->addWidget(m_lightnessText, 3, 3);
    layoutTextcolor->addWidget(m_labelRedText, 1, 4);
    layoutTextcolor->addWidget(m_redText, 1, 5);
    layoutTextcolor->addWidget(m_labelGreenText, 2, 4);
    layoutTextcolor->addWidget(m_greenText, 2, 5);
    layoutTextcolor->addWidget(m_labelBlueText, 3, 4);
    layoutTextcolor->addWidget(m_blueText, 3, 5);
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
    m_hue = new QSpinBox();
    m_saturation = new QSpinBox();
    m_lightness = new QSpinBox();
    m_labelHue = new QLabel("H");
    m_labelSaturation = new QLabel("S");
    m_labelLightness = new QLabel("L");
    m_red = new QSpinBox();
    m_green = new QSpinBox();
    m_blue = new QSpinBox();
    m_labelRed = new QLabel("R");
    m_labelGreen = new QLabel("G");
    m_labelBlue = new QLabel("B");
    m_hue->setVisible(false);
    m_saturation->setVisible(false);
    m_lightness->setVisible(false);
    m_red->setVisible(false);
    m_green->setVisible(false);
    m_blue->setVisible(false);
    m_labelHue->setVisible(false);
    m_labelSaturation->setVisible(false);
    m_labelLightness->setVisible(false);
    m_labelRed->setVisible(false);
    m_labelGreen->setVisible(false);
    m_labelBlue->setVisible(false);
    m_red->setMinimum(0);
    m_red->setMaximum(255);
    m_green->setMinimum(0);
    m_green->setMaximum(255);
    m_blue->setMinimum(0);
    m_blue->setMaximum(255);
    m_hue->setMinimum(0);
    m_hue->setMaximum(100);
    m_saturation->setMinimum(0);
    m_saturation->setMaximum(100);
    m_lightness->setMinimum(0);
    m_lightness->setMaximum(100);
    m_labelHue->setFixedWidth(15);
    m_labelSaturation->setFixedWidth(15);
    m_labelLightness->setFixedWidth(15);
    m_labelRed->setFixedWidth(15);
    m_labelGreen->setFixedWidth(15);
    m_labelBlue->setFixedWidth(15);

    m_hueBordercolorSlider = new QSlider();
    m_hueBordercolorSlider->setVisible(false);
    m_hueBordercolorSlider->setMinimum(0);
    m_hueBordercolorSlider->setMaximum(100);
    m_hueBordercolorSlider->setOrientation(Qt::Vertical);
    m_hueBordercolorSlider->setMaximumHeight(100);

    m_hueBorder = new QSpinBox();
    m_saturationBorder = new QSpinBox();
    m_lightnessBorder = new QSpinBox();
    m_labelHueBorder = new QLabel("H");
    m_labelSaturationBorder = new QLabel("S");
    m_labelLightnessBorder = new QLabel("L");
    m_redBorder = new QSpinBox();
    m_greenBorder = new QSpinBox();
    m_blueBorder = new QSpinBox();
    m_labelRedBorder = new QLabel("R");
    m_labelGreenBorder = new QLabel("G");
    m_labelBlueBorder = new QLabel("B");
    m_hueBorder->setVisible(false);
    m_saturationBorder->setVisible(false);
    m_lightnessBorder->setVisible(false);
    m_redBorder->setVisible(false);
    m_greenBorder->setVisible(false);
    m_blueBorder->setVisible(false);
    m_labelHueBorder->setVisible(false);
    m_labelSaturationBorder->setVisible(false);
    m_labelLightnessBorder->setVisible(false);
    m_labelRedBorder->setVisible(false);
    m_labelGreenBorder->setVisible(false);
    m_labelBlueBorder->setVisible(false);
    m_redBorder->setMinimum(0);
    m_redBorder->setMaximum(255);
    m_greenBorder->setMinimum(0);
    m_greenBorder->setMaximum(255);
    m_blueBorder->setMinimum(0);
    m_blueBorder->setMaximum(255);
    m_hueBorder->setMinimum(0);
    m_hueBorder->setMaximum(100);
    m_saturationBorder->setMinimum(0);
    m_saturationBorder->setMaximum(100);
    m_lightnessBorder->setMinimum(0);
    m_lightnessBorder->setMaximum(100);
    m_labelHueBorder->setFixedWidth(15);
    m_labelSaturationBorder->setFixedWidth(15);
    m_labelLightnessBorder->setFixedWidth(15);
    m_labelRedBorder->setFixedWidth(15);
    m_labelGreenBorder->setFixedWidth(15);
    m_labelBlueBorder->setFixedWidth(15);

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
    layoutColor->addWidget(m_brushcolor, 0, 2, 1, 4);
    layoutColor->addWidget(m_colorPicker, 1, 0, 3, 1);
    layoutColor->addWidget(m_hueColorSlider, 1, 1, 3, 1);
    layoutColor->addWidget(m_labelHue, 1, 2);
    layoutColor->addWidget(m_hue, 1, 3);
    layoutColor->addWidget(m_labelSaturation, 2, 2);
    layoutColor->addWidget(m_saturation, 2, 3);
    layoutColor->addWidget(m_labelLightness, 3, 2);
    layoutColor->addWidget(m_lightness, 3, 3);
    layoutColor->addWidget(m_labelRed, 1, 4);
    layoutColor->addWidget(m_red, 1, 5);
    layoutColor->addWidget(m_labelGreen, 2, 4);
    layoutColor->addWidget(m_green, 2, 5);
    layoutColor->addWidget(m_labelBlue, 3, 4);
    layoutColor->addWidget(m_blue, 3, 5);
    layoutColor->addWidget(labelBorder, 4, 0);
    layoutColor->addWidget(m_borderColorRect, 4, 1);
    layoutColor->addWidget(m_pencolor, 4, 2, 1, 4);
    layoutColor->addWidget(m_bordercolorPicker, 5, 0, 3, 1);
    layoutColor->addWidget(m_hueBordercolorSlider, 5, 1, 3, 1);
    layoutColor->addWidget(m_labelHueBorder, 5, 2);
    layoutColor->addWidget(m_hueBorder, 5, 3);
    layoutColor->addWidget(m_labelSaturationBorder, 6, 2);
    layoutColor->addWidget(m_saturationBorder, 6, 3);
    layoutColor->addWidget(m_labelLightnessBorder, 7, 2);
    layoutColor->addWidget(m_lightnessBorder, 7, 3);
    layoutColor->addWidget(m_labelRedBorder, 5, 4);
    layoutColor->addWidget(m_redBorder, 5, 5);
    layoutColor->addWidget(m_labelGreenBorder, 6, 4);
    layoutColor->addWidget(m_greenBorder, 6, 5);
    layoutColor->addWidget(m_labelBlueBorder, 7, 4);
    layoutColor->addWidget(m_blueBorder, 7, 5);
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
    connect(m_hue, SIGNAL(valueChanged(int)), this, SLOT(hueValueChanged(int)));
    connect(m_saturation, SIGNAL(valueChanged(int)), this, SLOT(saturationValueChanged(int)));
    connect(m_lightness, SIGNAL(valueChanged(int)), this, SLOT(lightnessValueChanged(int)));
    connect(m_red, SIGNAL(valueChanged(int)), this, SLOT(redValueChanged(int)));
    connect(m_green, SIGNAL(valueChanged(int)), this, SLOT(greenValueChanged(int)));
    connect(m_blue, SIGNAL(valueChanged(int)), this, SLOT(blueBorderValueChanged(int)));
    connect(m_hueBorder, SIGNAL(valueChanged(int)), this, SLOT(hueBorderValueChanged(int)));
    connect(m_saturationBorder, SIGNAL(valueChanged(int)), this, SLOT(saturationBorderValueChanged(int)));
    connect(m_lightnessBorder, SIGNAL(valueChanged(int)), this, SLOT(lightnessBorderValueChanged(int)));
    connect(m_redBorder, SIGNAL(valueChanged(int)), this, SLOT(redBorderValueChanged(int)));
    connect(m_greenBorder, SIGNAL(valueChanged(int)), this, SLOT(greenBorderValueChanged(int)));
    connect(m_blueBorder, SIGNAL(valueChanged(int)), this, SLOT(blueBorderValueChanged(int)));
    connect(m_hueText, SIGNAL(valueChanged(int)), this, SLOT(hueTextValueChanged(int)));
    connect(m_saturationText, SIGNAL(valueChanged(int)), this, SLOT(saturationTextValueChanged(int)));
    connect(m_lightnessText, SIGNAL(valueChanged(int)), this, SLOT(lightnessTextValueChanged(int)));
    connect(m_redText, SIGNAL(valueChanged(int)), this, SLOT(redTextValueChanged(int)));
    connect(m_greenText, SIGNAL(valueChanged(int)), this, SLOT(greenTextValueChanged(int)));
    connect(m_blueText, SIGNAL(valueChanged(int)), this, SLOT(blueTextValueChanged(int)));
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
        setTextcolorParts(m_textitem->textcolor());
    }

    m_rectangle = dynamic_cast<Rectangle*>(item);
    if(m_rectangle)
    {
        m_brushcolor->setText(m_rectangle->brush().color().name());
        m_pencolor->setText(m_rectangle->pen().color().name());
        expColor->setVisible(true);
        setColorParts(m_rectangle->brush().color());
        setBordercolorParts(m_rectangle->pen().color());
    }

    m_ellipse = dynamic_cast<Ellipse*>(item);
    if(m_ellipse)
    {
        m_brushcolor->setText(m_ellipse->brush().color().name());
        m_pencolor->setText(m_ellipse->pen().color().name());
        expColor->setVisible(true);
        setColorParts(m_ellipse->brush().color());
        setBordercolorParts(m_ellipse->pen().color());
    }
    expText->setVisible(m_textitem);
    expTextcolor->setVisible(m_textitem);

    connect(item, SIGNAL(sizeChanged(qreal,qreal)), this, SLOT(itemSizeChanged(qreal, qreal)));
    connect(item, SIGNAL(positionChanged(qreal,qreal)), this, SLOT(itemPositionChanged(qreal, qreal)));
}

void ItemPropertyEditor::itemSizeChanged(qreal width, qreal height)
{
    m_initializing = true;
    m_width->setValue(width);
    m_height->setValue(height);
    m_initializing = false;
}

void ItemPropertyEditor::itemPositionChanged(qreal x, qreal y)
{
    m_initializing = true;
    m_x->setValue(x);
    m_y->setValue(y);
    m_initializing = false;
}

void ItemPropertyEditor::idChanged(QString value)
{
    m_item->setId(value);
}

void ItemPropertyEditor::xChanged(int value)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new MoveItemCommand(value, m_item->y(), m_item->x(), m_item->y(), m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::yChanged(int value)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new MoveItemCommand(m_item->x(), value, m_item->x(), m_item->y(), m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::widthChanged(int value)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ResizeItemCommand(value, m_item->rect().height(), m_item->rect().width(), m_item->rect().height(), m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::heightChanged(int value)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ResizeItemCommand(m_item->rect().width(), value, m_item->rect().width(), m_item->rect().height(), m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::textChanged(QString value)
{
    m_textitem->setText(value);
}

void ItemPropertyEditor::textcolorChanged(QString value)
{
    QColor color = QColor(value);
    m_textitem->setTextcolor(color);
    m_textcolorRect->setColor(color);
    setTextcolorParts(color);
}

void ItemPropertyEditor::textcolorChanged(QColor value)
{
    m_textitem->setTextcolor(value);
    m_textcolor->setText(value.name());
    m_textcolorRect->setColor(value);
    setTextcolorParts(value);
}

void ItemPropertyEditor::colorChanged(QString value)
{
    QColor color = QColor(value);
    if(m_rectangle)
        m_rectangle->setBrush(QBrush(color));

    if(m_ellipse)
        m_ellipse->setBrush(QBrush(color));

    m_colorRect->setColor(color);
    setColorParts(color);
}

void ItemPropertyEditor::colorChanged(QColor value)
{
    if(m_rectangle)
        m_rectangle->setBrush(QBrush(value));

    if(m_ellipse)
        m_ellipse->setBrush(QBrush(value));
    m_brushcolor->setText(value.name());
    m_colorRect->setColor(value);
    setColorParts(value);
}

void ItemPropertyEditor::setColorParts(QColor value)
{
    m_initializing = true;
    m_hue->setValue(value.hslHueF() * 100);
    m_hueColorSlider->setValue(value.hslHueF() * 100);
    m_saturation->setValue(value.hslSaturationF() * 100);
    m_lightness->setValue(value.lightnessF() * 100);
    m_red->setValue(value.red());
    m_green->setValue(value.green());
    m_blue->setValue(value.blue());
    m_initializing = false;
}

void ItemPropertyEditor::setBordercolorParts(QColor value)
{
    m_initializing = true;
    m_hueBorder->setValue(value.hslHueF() * 100);
    m_hueBordercolorSlider->setValue(value.hslHueF() * 100);
    m_saturationBorder->setValue(value.hslSaturationF() * 100);
    m_lightnessBorder->setValue(value.lightnessF() * 100);
    m_redBorder->setValue(value.red());
    m_greenBorder->setValue(value.green());
    m_blueBorder->setValue(value.blue());
    m_initializing = false;
}

void ItemPropertyEditor::setTextcolorParts(QColor value)
{
    m_initializing = true;
    m_hueText->setValue(value.hslHueF() * 100);
    m_hueTextColorSlider->setValue(value.hslHueF() * 100);
    m_saturationText->setValue(value.hslSaturationF() * 100);
    m_lightnessText->setValue(value.lightnessF() * 100);
    m_redText->setValue(value.red());
    m_greenText->setValue(value.green());
    m_blueText->setValue(value.blue());
    m_initializing = false;
}

void ItemPropertyEditor::borderColorChanged(QString value)
{
    QColor color = QColor(value);
    if(m_rectangle)
        m_rectangle->setPen(QPen(color));

    if(m_ellipse)
        m_ellipse->setPen(QPen(color));
    m_borderColorRect->setColor(color);
    setBordercolorParts(color);
}

void ItemPropertyEditor::bordercolorChanged(QColor value)
{
    if(m_rectangle)
        m_rectangle->setPen(QPen(value));

    if(m_ellipse)
        m_ellipse->setPen(QPen(value));
    m_pencolor->setText(value.name());
    m_borderColorRect->setColor(value);
    setBordercolorParts(value);
}

void ItemPropertyEditor::hueChanged(int value)
{
    m_colorPicker->setHue((qreal)value / 100.0);
    m_hue->setValue(value);
}

void ItemPropertyEditor::hueValueChanged(int value)
{
    if(m_initializing)
        return;
    m_hueColorSlider->setValue(value);
    QColor color = QColor::fromHslF((qreal)value / 100, (qreal)m_saturation->value() / 100, (qreal)m_lightness->value() / 100, 1.0);
    m_brushcolor->setText(color.name());
    m_colorRect->setColor(color);
}

void ItemPropertyEditor::saturationValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromHslF((qreal)m_hue->value() / 100, (qreal)value / 100, (qreal)m_lightness->value() / 100, 1.0);
    m_brushcolor->setText(color.name());
    m_colorRect->setColor(color);
}

void ItemPropertyEditor::lightnessValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromHslF((qreal)m_hue->value() / 100, (qreal)m_saturation->value() / 100, (qreal)value / 100, 1.0);
    m_brushcolor->setText(color.name());
    m_colorRect->setColor(color);
}

void ItemPropertyEditor::redValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(value, m_green->value(), m_blue->value());
    m_brushcolor->setText(color.name());
    m_colorRect->setColor(color);
}

void ItemPropertyEditor::greenValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(m_red->value(), value, m_blue->value());
    m_brushcolor->setText(color.name());
    m_colorRect->setColor(color);
}

void ItemPropertyEditor::blueValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(m_red->value(), m_green->value(), value);
    m_brushcolor->setText(color.name());
    m_colorRect->setColor(color);
}

void ItemPropertyEditor::hueBorderChanged(int value)
{
    m_bordercolorPicker->setHue((qreal)value / 100.0);
    m_hueBorder->setValue(value);
}

void ItemPropertyEditor::hueBorderValueChanged(int value)
{
    if(m_initializing)
        return;
    m_hueBordercolorSlider->setValue(value);
    QColor color = QColor::fromHslF((qreal)value / 100, (qreal)m_saturationBorder->value() / 100, (qreal)m_lightnessBorder->value() / 100, 1.0);
    m_pencolor->setText(color.name());
    m_borderColorRect->setColor(color);
}

void ItemPropertyEditor::saturationBorderValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromHslF((qreal)m_hueBorder->value() / 100, (qreal)value / 100, (qreal)m_lightnessBorder->value() / 100, 1.0);
    m_pencolor->setText(color.name());
    m_borderColorRect->setColor(color);
}

void ItemPropertyEditor::lightnessBorderValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromHslF((qreal)m_hueBorder->value() / 100, (qreal)m_saturationBorder->value() / 100, (qreal)value / 100, 1.0);
    m_pencolor->setText(color.name());
    m_borderColorRect->setColor(color);
}

void ItemPropertyEditor::redBorderValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(value, m_greenBorder->value(), m_blueBorder->value());
    m_pencolor->setText(color.name());
    m_borderColorRect->setColor(color);
}

void ItemPropertyEditor::greenBorderValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(m_redBorder->value(), value, m_blueBorder->value());
    m_pencolor->setText(color.name());
    m_borderColorRect->setColor(color);

}

void ItemPropertyEditor::blueBorderValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(m_redBorder->value(), m_greenBorder->value(), value);
    m_pencolor->setText(color.name());
    m_borderColorRect->setColor(color);
}

void ItemPropertyEditor::hueTextcolorChanged(int value)
{
    m_textcolorpicker->setHue((qreal)value / 100.0);
    m_hueText->setValue(value);
}

void ItemPropertyEditor::hueTextValueChanged(int value)
{
    if(m_initializing)
        return;
    m_hueColorSlider->setValue(value);
    QColor color = QColor::fromHslF((qreal)value / 100, (qreal)m_saturationText->value() / 100, (qreal)m_lightnessText->value() / 100, 1.0);
    m_textcolor->setText(color.name());
    m_textcolorRect->setColor(color);
}

void ItemPropertyEditor::saturationTextValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromHslF((qreal)m_hueText->value() / 100, (qreal)value / 100, (qreal)m_lightnessText->value() / 100, 1.0);
    m_textcolor->setText(color.name());
    m_textcolorRect->setColor(color);
}

void ItemPropertyEditor::lightnessTextValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromHslF((qreal)m_hueText->value() / 100, (qreal)m_saturationText->value() / 100, (qreal)value / 100, 1.0);
    m_textcolor->setText(color.name());
    m_textcolorRect->setColor(color);
}

void ItemPropertyEditor::redTextValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(value, m_greenText->value(), m_blueText->value());
    m_textcolor->setText(color.name());
    m_textcolorRect->setColor(color);
}

void ItemPropertyEditor::greenTextValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(m_redText->value(), value, m_blueText->value());
    m_textcolor->setText(color.name());
    m_textcolorRect->setColor(color);

}

void ItemPropertyEditor::blueTextValueChanged(int value)
{
    if(m_initializing)
        return;
    QColor color = QColor::fromRgb(m_redText->value(), m_greenText->value(), value);
    m_textcolor->setText(color.name());
    m_textcolorRect->setColor(color);

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
    m_labelHue->setVisible(!visible);
    m_labelSaturation->setVisible(!visible);
    m_labelLightness->setVisible(!visible);
    m_hue->setVisible(!visible);
    m_saturation->setVisible(!visible);
    m_lightness->setVisible(!visible);
    m_labelRed->setVisible(!visible);
    m_labelGreen->setVisible(!visible);
    m_labelBlue->setVisible(!visible);
    m_red->setVisible(!visible);
    m_green->setVisible(!visible);
    m_blue->setVisible(!visible);

    if(!visible)
    {
        m_bordercolorPicker->setVisible(visible);
        m_hueBordercolorSlider->setVisible(visible);
        m_labelHueBorder->setVisible(visible);
        m_labelSaturationBorder->setVisible(visible);
        m_labelLightnessBorder->setVisible(visible);
        m_hueBorder->setVisible(visible);
        m_saturationBorder->setVisible(visible);
        m_lightnessBorder->setVisible(visible);
        m_labelRedBorder->setVisible(visible);
        m_labelGreenBorder->setVisible(visible);
        m_labelBlueBorder->setVisible(visible);
        m_redBorder->setVisible(visible);
        m_greenBorder->setVisible(visible);
        m_blueBorder->setVisible(visible);
    }
}

void ItemPropertyEditor::borderColorRectClicked()
{
    bool visible = m_bordercolorPicker->isVisible();

    m_bordercolorPicker->setVisible(!visible);
    m_hueBordercolorSlider->setVisible(!visible);
    m_labelHueBorder->setVisible(!visible);
    m_labelSaturationBorder->setVisible(!visible);
    m_labelLightnessBorder->setVisible(!visible);
    m_hueBorder->setVisible(!visible);
    m_saturationBorder->setVisible(!visible);
    m_lightnessBorder->setVisible(!visible);
    m_labelRedBorder->setVisible(!visible);
    m_labelGreenBorder->setVisible(!visible);
    m_labelBlueBorder->setVisible(!visible);
    m_redBorder->setVisible(!visible);
    m_greenBorder->setVisible(!visible);
    m_blueBorder->setVisible(!visible);

    if(!visible)
    {
        m_colorPicker->setVisible(visible);
        m_hueColorSlider->setVisible(visible);
        m_labelHue->setVisible(visible);
        m_labelSaturation->setVisible(visible);
        m_labelLightness->setVisible(visible);
        m_hue->setVisible(visible);
        m_saturation->setVisible(visible);
        m_lightness->setVisible(visible);
        m_labelRed->setVisible(visible);
        m_labelGreen->setVisible(visible);
        m_labelBlue->setVisible(visible);
        m_red->setVisible(visible);
        m_green->setVisible(visible);
        m_blue->setVisible(visible);
    }
}

void ItemPropertyEditor::textColorRectClicked()
{
    bool visible = m_textcolorpicker->isVisible();

    m_textcolorpicker->setVisible(!visible);
    m_hueTextColorSlider->setVisible(!visible);
    m_labelHueText->setVisible(!visible);
    m_labelSaturationText->setVisible(!visible);
    m_labelLightnessText->setVisible(!visible);
    m_hueText->setVisible(!visible);
    m_saturationText->setVisible(!visible);
    m_lightnessText->setVisible(!visible);
    m_labelRedText->setVisible(!visible);
    m_labelGreenText->setVisible(!visible);
    m_labelBlueText->setVisible(!visible);
    m_redText->setVisible(!visible);
    m_greenText->setVisible(!visible);
    m_blueText->setVisible(!visible);
}
