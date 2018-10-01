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
#include "animationitem.h"
#include "animationscene.h"
#include "xmlhighlighter.h"
#include "propertyeditorinterface.h"
#include "svgattributeeditor.h"
#include "xmleditor.h"
#include "text.h"
#include "rectangle.h"
#include "timeline.h"
#include "ellipse.h"
#include "flatbutton.h"
#include "expander.h"
#include "coloreditor.h"
#include "vectorgraphic.h"
#include "commands.h"
#include <QPushButton>
#include <QComboBox>
#include <QFontDatabase>
#include <QRadioButton>

ItemPropertyEditor::ItemPropertyEditor(Timeline *timeline)
{
    m_timeline = timeline;
    m_rectangle = nullptr;
    m_ellipse = nullptr;
    m_vector = nullptr;
    m_initializing = false;

    m_additionalPropertyBox = new QVBoxLayout();
    QVBoxLayout *scrollbox = new QVBoxLayout();
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
    FlatButton *addXKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addXKeyframe->setMaximumWidth(9);
    addXKeyframe->setToolTip("Add keyframe for Left");
    FlatButton *addYKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addYKeyframe->setMaximumWidth(9);
    addYKeyframe->setToolTip("Add keyframe for Top");
    FlatButton *addWidthKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addWidthKeyframe->setMaximumWidth(9);
    addWidthKeyframe->setToolTip("Add keyframe for Width");
    FlatButton *addHeightKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
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

    Expander *expTrans = new Expander("Transformation");
    QGridLayout *layoutTrans = new QGridLayout();
    QLabel *labelScale = new QLabel("Scale");
    QLabel *labelShear = new QLabel("Shear");
    QLabel *labelTranslate = new QLabel("Translate");
    QLabel *labelRotate = new QLabel("Rotate");
    QLabel *labelScaleX = new QLabel("X");
    QLabel *labelScaleY = new QLabel("Y");
    QLabel *labelShearX = new QLabel("X");
    QLabel *labelShearY = new QLabel("Y");
    QLabel *labelTransX = new QLabel("X");
    QLabel *labelTransY = new QLabel("Y");
    m_rotationX = new QRadioButton("X");
    m_rotationY = new QRadioButton("Y");
    m_rotationZ = new QRadioButton("Z");
    m_scaleX = new QSpinBox();
    m_scaleX->setMinimum(-10000);
    m_scaleX->setMaximum(10000);
    m_scaleY = new QSpinBox();
    m_scaleY->setMinimum(-10000);
    m_scaleY->setMaximum(10000);
    m_shearX = new QSpinBox();
    m_shearX->setMinimum(-10000);
    m_shearX->setMaximum(10000);
    m_shearY = new QSpinBox();
    m_shearY->setMinimum(-10000);
    m_shearY->setMaximum(10000);
    m_transX = new QSpinBox();
    m_transX->setMinimum(-10000);
    m_transX->setMaximum(10000);
    m_transY = new QSpinBox();
    m_transY->setMinimum(-10000);
    m_transY->setMaximum(10000);
    m_rotate = new QSpinBox();
    m_rotate->setMinimum(-10000);
    m_rotate->setMaximum(10000);

    FlatButton *addScaleXKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addScaleXKeyframe->setMaximumWidth(9);
    addScaleXKeyframe->setToolTip("Add keyframe for scale X");
    FlatButton *addScaleYKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addScaleYKeyframe->setMaximumWidth(9);
    addScaleYKeyframe->setToolTip("Add keyframe for scale Y");
    FlatButton *addShearXKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addShearXKeyframe->setMaximumWidth(9);
    addShearXKeyframe->setToolTip("Add keyframe for shear X");
    FlatButton *addShearYKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addShearYKeyframe->setMaximumWidth(9);
    addShearYKeyframe->setToolTip("Add keyframe for shear Y");
    FlatButton *addTransXKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addTransXKeyframe->setMaximumWidth(9);
    addTransXKeyframe->setToolTip("Add keyframe for translate X");
    FlatButton *addTransYKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addTransYKeyframe->setMaximumWidth(9);
    addTransYKeyframe->setToolTip("Add keyframe for translate Y");
    FlatButton *addRotateKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
    addRotateKeyframe->setMaximumWidth(9);
    addRotateKeyframe->setToolTip("Add keyframe for rotation");
    layoutTrans->addWidget(labelScale, 0, 0);
    layoutTrans->addWidget(labelScaleX, 0, 1);
    layoutTrans->addWidget(addScaleXKeyframe, 0, 2);
    layoutTrans->addWidget(m_scaleX, 0, 3);
    layoutTrans->addWidget(labelScaleY, 0, 4);
    layoutTrans->addWidget(addScaleYKeyframe, 0, 5);
    layoutTrans->addWidget(m_scaleY, 0, 6);
    layoutTrans->addWidget(labelShear, 1, 0);
    layoutTrans->addWidget(labelShearX, 1, 1);
    layoutTrans->addWidget(addShearXKeyframe, 1, 2);
    layoutTrans->addWidget(m_shearX, 1, 3);
    layoutTrans->addWidget(labelShearY, 1, 4);
    layoutTrans->addWidget(addShearYKeyframe, 1, 5);
    layoutTrans->addWidget(m_shearY, 1, 6);
    layoutTrans->addWidget(labelTranslate, 2, 0);
    layoutTrans->addWidget(labelTransX, 2, 1);
    layoutTrans->addWidget(addTransXKeyframe, 2, 2);
    layoutTrans->addWidget(m_transX, 2, 3);
    layoutTrans->addWidget(labelTransY, 2, 4);
    layoutTrans->addWidget(addTransYKeyframe, 2, 5);
    layoutTrans->addWidget(m_transY, 2, 6);
    layoutTrans->addWidget(labelRotate, 3, 0);
    layoutTrans->addWidget(addRotateKeyframe, 3, 2);
    layoutTrans->addWidget(m_rotate, 3, 3);
    layoutTrans->addWidget(m_rotationX, 3, 4);
    layoutTrans->addWidget(m_rotationY, 3, 5);
    layoutTrans->addWidget(m_rotationZ, 3, 6);
    expTrans->addLayout(layoutTrans);
    vbox->addWidget(expTrans);

    expText = new Expander("Text");
    expText->setVisible(false);
    QGridLayout *layoutText = new QGridLayout();
    QLabel *labelText = new QLabel("Text");
    QLabel *labelFont = new QLabel("Font");
    QLabel *labelFontsize = new QLabel("Size");
    QLabel *labelStyle = new QLabel("Style");
    labelStyle->setFixedWidth(40);
    m_fontSize = new QComboBox();
    m_fontSize->setEditable(true);
    m_style = new QComboBox();
    m_font = new QComboBox();
    QStringList fonts = m_fontdatabase.families();
    for(int i=0; i < fonts.count(); i++)
    {
        m_font->addItem(fonts.at(i), QVariant(fonts.at(i)));
    }
    m_text = new QLineEdit();
    layoutText->addWidget(labelText, 0, 0);
    layoutText->addWidget(m_text, 0, 1, 1, 3);
    layoutText->addWidget(labelFont, 1, 0);
    layoutText->addWidget(m_font, 1, 1, 1, 3);
    layoutText->addWidget(labelFontsize, 2, 0);
    layoutText->addWidget(m_fontSize, 2, 1);
    layoutText->addWidget(labelStyle, 2, 2);
    layoutText->addWidget(m_style, 2, 3);
    expText->addLayout(layoutText);
    vbox->addWidget(expText);

    expTextcolor = new Expander("Textcolor");
    expTextcolor->setVisible(false);

    QVBoxLayout *layoutTextcolor = new QVBoxLayout();
    textcolorEditor = new ColorEditor("Color");
    layoutTextcolor->addWidget(textcolorEditor);
    expTextcolor->addLayout(layoutTextcolor);
    vbox->addWidget(expTextcolor);

    expColor = new Expander("Color");
    expColor->setVisible(false);
    colorEditor = new ColorEditor("Brush");
    borderColorEditor = new ColorEditor("Pen");
    QVBoxLayout *layoutColor = new QVBoxLayout();
    layoutColor->addWidget(colorEditor);
    layoutColor->addWidget(borderColorEditor);
    expColor->addLayout(layoutColor);
    vbox->addWidget(expColor);

    Expander *expOpacity = new Expander("Visibility");
    QGridLayout *layoutOpacity = new QGridLayout();
    FlatButton *addOpacityKeyframe = new FlatButton(":/images/raute.png", ":/images/raute-hover.png");
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

    expSvg = new Expander("SVG");
    m_svgText = new XmlEditor;

    QVBoxLayout *vboxSvg = new QVBoxLayout;
    m_vboxAttributeEditors = new QVBoxLayout;
    FlatButton *plus = new FlatButton(":/images/plus_normal.png", ":/images/plus_hover.png");
    plus->setToolTip("Add attribute to animate");
    plus->setMaximumWidth(13);

    vboxSvg->addWidget(m_svgText);
    vboxSvg->addLayout(m_vboxAttributeEditors);
    vboxSvg->addWidget(plus, 0, Qt::AlignLeft);
    expSvg->addLayout(vboxSvg);
    vbox->addWidget(expSvg);

    scrollbox->addLayout(vbox);
    scrollbox->addLayout(m_additionalPropertyBox);
    scrollbox->addStretch();
    QWidget *scrollContent = new QWidget();
    scrollContent->setLayout(scrollbox);
    QScrollArea *scroll = new QScrollArea();
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setWidget(scrollContent);
    scroll->setWidgetResizable(true);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(scroll);
    setLayout(layout);

    connect(m_id, SIGNAL(editingFinished()), this, SLOT(idChanged()));
    connect(m_x, SIGNAL(valueChanged(int)), this, SLOT(xChanged(int)));
    connect(m_y, SIGNAL(valueChanged(int)), this, SLOT(yChanged(int)));
    connect(m_width, SIGNAL(valueChanged(int)), this, SLOT(widthChanged(int)));
    connect(m_height, SIGNAL(valueChanged(int)), this, SLOT(heightChanged(int)));
    connect(m_text, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(addXKeyframe, SIGNAL(clicked()), this, SLOT(addLeftKeyFrame()));
    connect(addYKeyframe, SIGNAL(clicked()), this, SLOT(addTopKeyFrame()));
    connect(addWidthKeyframe, SIGNAL(clicked()), this, SLOT(addWidthKeyFrame()));
    connect(addHeightKeyframe, SIGNAL(clicked()), this, SLOT(addHeightKeyFrame()));
    connect(m_opacity, SIGNAL(sliderReleased()), this, SLOT(opacitySliderReleased()));
    connect(m_opacity, SIGNAL(valueChanged(int)), this, SLOT(opacitySliderReleased()));
    connect(m_opacityText, SIGNAL(valueChanged(int)), this, SLOT(opacityTextChanged(int)));
    connect(addOpacityKeyframe, SIGNAL(clicked()), this, SLOT(addOpacityKeyFrame()));
    connect(colorEditor, SIGNAL(colorChanged(QColor)), this, SLOT(colorChanged(QColor)));
    connect(colorEditor, SIGNAL(addKeyframe()), this, SLOT(addBrushKeyFrame()));
    connect(borderColorEditor, SIGNAL(colorChanged(QColor)), this, SLOT(borderColorChanged(QColor)));
    connect(borderColorEditor, SIGNAL(addKeyframe()), this, SLOT(addPenKeyFrame()));
    connect(textcolorEditor, SIGNAL(colorChanged(QColor)), this, SLOT(textColorChanged(QColor)));
    connect(textcolorEditor, SIGNAL(addKeyframe()), this, SLOT(addTextColorKeyFrame()));
    connect(m_font, SIGNAL(currentIndexChanged(int)), this, SLOT(fontFamilyChanged(int)));
    connect(m_style, SIGNAL(currentIndexChanged(int)), this, SLOT(fontStyleChanged(int)));
    connect(m_fontSize, SIGNAL(currentTextChanged(QString)), this, SLOT(fontSizeChanged()));
    connect(m_svgText, SIGNAL(editingFinished()), this, SLOT(svgTextChanged()));
    connect(plus, SIGNAL(clicked()), this, SLOT(addSvgAttributeEditor()));

    connect(m_scaleX, SIGNAL(valueChanged(int)), this, SLOT(scaleXChanged(int)));
    connect(m_scaleY, SIGNAL(valueChanged(int)), this, SLOT(scaleYChanged(int)));
    connect(m_shearX, SIGNAL(valueChanged(int)), this, SLOT(shearXChanged(int)));
    connect(m_shearY, SIGNAL(valueChanged(int)), this, SLOT(shearYChanged(int)));
    connect(m_transX, SIGNAL(valueChanged(int)), this, SLOT(transXChanged(int)));
    connect(m_transY, SIGNAL(valueChanged(int)), this, SLOT(transYChanged(int)));
    connect(m_rotate, SIGNAL(valueChanged(int)), this, SLOT(rotationChanged(int)));
    connect(m_rotationX, SIGNAL(toggled(bool)), this, SLOT(rotationXChanged(bool)));
    connect(m_rotationY, SIGNAL(toggled(bool)), this, SLOT(rotationYChanged(bool)));
    connect(m_rotationZ, SIGNAL(toggled(bool)), this, SLOT(rotationZChanged(bool)));
}

void ItemPropertyEditor::fontFamilyChanged(int index)
{
    QString family =  m_font->itemText(index);
    QString style = m_style->currentText();
    int size = m_textitem->font().pointSize();
    QStringList styles = m_fontdatabase.styles(family);

    m_initializing = true;
    m_style->clear();

    for(int j=0; j < styles.count(); j++)
    {
        m_style->addItem(styles.at(j), QVariant(styles.at(j)));
    }
    m_style->setCurrentText(style);
    m_fontSize->clear();
    QList<int> sizes = m_fontdatabase.smoothSizes(family, m_style->currentText());
    for(int k=0; k < sizes.count(); k++)
    {
        m_fontSize->addItem(QString::number(sizes.at(k)), QVariant(sizes.at(k)));
    }
    m_fontSize->setCurrentText(QString::number(size));
    m_initializing = false;

    QFont font = m_fontdatabase.font(family, m_style->currentText(), m_fontSize->currentText().toInt());
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeFontCommand(font, m_textitem->font(), m_textitem);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::fontStyleChanged(int index)
{
    if(m_initializing)
        return;
    QString family =  m_font->itemText(m_font->currentIndex());
    QString style = m_style->itemText(index);
    int size = m_textitem->font().pointSize();

    m_initializing = true;
    m_fontSize->clear();
    QList<int> sizes = m_fontdatabase.smoothSizes(family, style);
    for(int k=0; k < sizes.count(); k++)
    {
        m_fontSize->addItem(QString::number(sizes.at(k)), QVariant(sizes.at(k)));
    }
    m_fontSize->setCurrentText(QString::number(size));
    m_initializing = false;

    QFont font = m_fontdatabase.font(m_font->currentText(), style, m_fontSize->currentText().toInt());
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeFontCommand(font, m_textitem->font(), m_textitem);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::fontSizeChanged()
{
    if(m_initializing)
        return;

    QFont font = m_fontdatabase.font(m_font->currentText(), m_style->currentText(), m_fontSize->currentText().toInt());
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeFontCommand(font, m_textitem->font(), m_textitem);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::svgAttributeValueChanged(QString attributeName, QString value)
{
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeAttributeCommand(attributeName, value, m_vector->attributes().value(attributeName), as, m_vector);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::svgTextChanged()
{
    if(m_initializing)
        return;

    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeSvgCommand(m_svgText->toPlainText().toUtf8(), m_vector->getData(), m_vector);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::addSvgAttributeEditor()
{
    addSvgAttributeEditor("", 0);
}

void ItemPropertyEditor::svgEditorRemoveClicked(SvgAttributeEditor *editor)
{
    if(editor->isValid())
    {
        AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
        if(as)
        {
            QUndoStack *undoStack = as->undoStack();
            QUndoCommand *cmd = new RemoveAttributeCommand(editor->attributeName(), editor->value(), m_vector);
            undoStack->push(cmd);
        }
    }
    else
        m_vector->removeAttribute(editor->attributeName());
    m_vboxAttributeEditors->removeWidget(editor);
    delete editor;
}

void ItemPropertyEditor::svgEditorAddKeyframeClicked(SvgAttributeEditor *editor)
{
    emit addKeyFrame(m_vector, editor->attributeName(), QVariant(editor->value()));
}

void ItemPropertyEditor::svgAttributeAdded()
{
    reloadAttributes();
}

void ItemPropertyEditor::svgDataChanged()
{
    m_svgText->setPlainText(QString::fromUtf8(m_vector->getData()));
}

void ItemPropertyEditor::addBrushKeyFrame()
{
    emit addKeyFrame(m_item, "brushColor", QVariant(colorEditor->color()));
}

void ItemPropertyEditor::addPenKeyFrame()
{
    emit addKeyFrame(m_item, "penColor", QVariant(borderColorEditor->color()));
}

void ItemPropertyEditor::addTextColorKeyFrame()
{
    emit addKeyFrame(m_item, "textColor", QVariant(textcolorEditor->color()));
}

void ItemPropertyEditor::scaleXChanged(int value)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeScaleXCommand((double)value/ 100.0, m_item->scaleX(), as, m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::scaleYChanged(int value)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeScaleYCommand((double)value/ 100.0, m_item->scaleY(), as, m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::shearXChanged(int value)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeShearXCommand((double)value/ 100.0, m_item->scaleX(), as, m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::shearYChanged(int value)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeShearYCommand((double)value/ 100.0, m_item->scaleX(), as, m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::transXChanged(int value)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeTransXCommand((double)value, m_item->scaleX(), as, m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::transYChanged(int value)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeTransYCommand((double)value, m_item->scaleX(), as, m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::rotationChanged(int value)
{
    QString rotation;
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        if(m_rotationX->isChecked())
            rotation = "X";
        else if(m_rotationY->isChecked())
            rotation = "Y";
        else if(m_rotationZ->isChecked())
            rotation = "Z";
        rotation += QString::number(value);
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeRotationCommand(rotation, m_item->rotation(), as, m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::rotationXChanged(bool)
{
    rotationChanged(m_rotate->value());
}

void ItemPropertyEditor::rotationYChanged(bool)
{
    rotationChanged(m_rotate->value());
}

void ItemPropertyEditor::rotationZChanged(bool)
{
    rotationChanged(m_rotate->value());
}

void ItemPropertyEditor::svgAttributeNameChanged(QString oldName, QString newName)
{
    if(m_initializing)
        return;
    m_vector->changeAttributeName(oldName, newName);
}

void ItemPropertyEditor::colorChanged(QColor color)
{
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeColorCommand(color, m_item->brush().color(), as, m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::borderColorChanged(QColor color)
{
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangePenCommand(color, m_item->pen().color(), as, m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::textColorChanged(QColor color)
{
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeTextcolorCommand(color, m_textitem->textColor(), as, m_textitem);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::addLeftKeyFrame()
{
    emit addKeyFrame(m_item, "left", QVariant((double)m_x->value()));
}

void ItemPropertyEditor::addTopKeyFrame()
{
    emit addKeyFrame(m_item, "top", QVariant((double)m_y->value()));
}

void ItemPropertyEditor::addWidthKeyFrame()
{
    emit addKeyFrame(m_item, "width", QVariant((double)m_width->value()));
}

void ItemPropertyEditor::addHeightKeyFrame()
{
    emit addKeyFrame(m_item, "height", QVariant((double)m_height->value()));
}

void ItemPropertyEditor::addOpacityKeyFrame()
{
    emit addKeyFrame(m_item, "opacity", QVariant(m_opacity->value()));
}

void ItemPropertyEditor::reloadAttributes()
{
    // remove all editors first
    for(int i = m_vboxAttributeEditors->count() - 1; i >= 0; i--)
    {
        QWidget *editor = m_vboxAttributeEditors->itemAt(i)->widget();
        m_vboxAttributeEditors->removeWidget(editor);
        delete editor;
    }
    if(m_vector->attributes().count() == 0)
        addSvgAttributeEditor();
    else
    {
        foreach(QString name, m_vector->attributes().keys())
        {
            addSvgAttributeEditor(name, m_vector->attributes().value(name));
        }
    }
}

void ItemPropertyEditor::setItem(AnimationItem *item)
{
    m_initializing = true;
    m_item = item;
    expColor->setVisible(false);
    expSvg->setVisible(false);
    m_x->setValue(m_item->x());
    m_y->setValue(m_item->y());
    m_width->setValue(m_item->rect().width());
    m_height->setValue(m_item->rect().height());
    m_typ->setText(m_item->typeName());
    m_id->setText(item->id());
    m_opacity->setValue(item->opacity());
    m_opacityText->setValue(item->opacity());
    m_scaleX->setValue(item->scaleX() * 100);
    m_scaleY->setValue(item->scaleY() * 100);
    m_shearX->setValue(item->shearX() * 100);
    m_shearY->setValue(item->shearY() * 100);
    m_transX->setValue(item->transX());
    m_transY->setValue(item->transY());
    m_rotate->setValue(item->rotation().right(item->rotation().size() - 1).toInt());
    if(item->rotation().startsWith("Z"))
        m_rotationZ->setChecked(true);
    else if(item->rotation().startsWith("Y"))
        m_rotationY->setChecked(true);
    else if(item->rotation().startsWith("X"))
        m_rotationX->setChecked(true);


    // delete all additional properties from prior usage
    QLayoutItem *litem;
    while((litem = m_additionalPropertyBox->takeAt(0)) != NULL)
    {
        delete litem->widget();
        delete litem;
    }
    // then fill in the additional properties
    QList<PropertyEditorInterface*> *list = item->getPropertyEditors();
    if(list)
    {
        for(int i = 0; i < list->count(); i++)
        {
            PropertyEditorInterface *pei = list->at(i);
            pei->setItem(item);
            connect(pei, SIGNAL(addKeyFrame(AnimationItem*,QString,QVariant)), m_timeline, SLOT(addKeyFrame(AnimationItem*,QString,QVariant)));
            m_additionalPropertyBox->addWidget(pei);
        }
        delete list;
    }

    if(item->hasBrushAndPen())
    {
        expColor->setVisible(true);
        colorEditor->setColor(item->brush().color());
        borderColorEditor->setColor(item->pen().color());
    }

    m_textitem = dynamic_cast<Text*>(item);
    if(m_textitem)
    {
        m_text->setText(m_textitem->text());
        textcolorEditor->setColor(m_textitem->textColor());
        QFont f = m_textitem->font();
        m_font->setCurrentText(f.family());
        m_style->setCurrentText(f.styleName());
        m_fontSize->setCurrentText(QString::number(f.pointSize()));
        connect(m_textitem, SIGNAL(textcolorChanged(QColor)), this, SLOT(textcolorChanged(QColor)));
    }

    m_rectangle = dynamic_cast<Rectangle*>(item);
    m_ellipse = dynamic_cast<Ellipse*>(item);

    m_vector = dynamic_cast<Vectorgraphic*>(item);
    if(m_vector)
    {
        QString vec = QString::fromUtf8(m_vector->getData());
        expSvg->setVisible(true);
        m_svgText->setPlainText(vec);

        reloadAttributes();

        connect(m_vector, SIGNAL(attributeAdded()), this, SLOT(svgAttributeAdded()));
        connect(m_vector, SIGNAL(dataChanged()), this, SLOT(svgDataChanged()));
    }

    expText->setVisible(m_textitem);
    expTextcolor->setVisible(m_textitem);
    m_initializing = false;

    connect(item, SIGNAL(idChanged(AnimationItem*,QString)), this, SLOT(idChanged(AnimationItem*,QString)));
    connect(item, SIGNAL(sizeChanged(qreal,qreal)), this, SLOT(itemSizeChanged(qreal, qreal)));
    connect(item, SIGNAL(positionChanged(qreal,qreal)), this, SLOT(itemPositionChanged(qreal, qreal)));
    connect(item, SIGNAL(brushChanged(QColor)), this, SLOT(brushChanged(QColor)));
    connect(item, SIGNAL(penChanged(QColor)), this, SLOT(penChanged(QColor)));
    connect(item, SIGNAL(opacityChanged(int)), this, SLOT(opacityChanged(int)));
}

void ItemPropertyEditor::brushChanged(QColor color)
{
    colorEditor->setColor(color);
}

void ItemPropertyEditor::penChanged(QColor color)
{
    borderColorEditor->setColor(color);
}

void ItemPropertyEditor::textcolorChanged(QColor color)
{
    textcolorEditor->setColor(color);
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

void ItemPropertyEditor::idChanged(AnimationItem*, QString id)
{
    m_initializing = true;
    m_id->setText(id);
    m_initializing = false;
}

void ItemPropertyEditor::idChanged()
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeIdCommand(m_id->text(), m_item->id(), m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::xChanged(int value)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new MoveItemCommand(value, m_item->y(), m_item->x(), m_item->y(), as, m_item);
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
        QUndoCommand *cmd = new MoveItemCommand(m_item->x(), value, m_item->x(), m_item->y(), as, m_item);
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
        QUndoCommand *cmd = new ResizeItemCommand(value, m_item->rect().height(), m_item->rect().width(), m_item->rect().height(), as, m_item);
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
        QUndoCommand *cmd = new ResizeItemCommand(m_item->rect().width(), value, m_item->rect().width(), m_item->rect().height(), as, m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::textChanged(QString value)
{
    m_textitem->setText(value);
}

void ItemPropertyEditor::changeBrush(QColor color)
{
    if(m_initializing || !color.isValid())
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeColorCommand(color, m_item->brush().color(), as, m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::opacitySliderReleased()
{
    changeOpacity(m_opacity->value());
}

void ItemPropertyEditor::opacityTextChanged(int value)
{
    changeOpacity(value);
}

void ItemPropertyEditor::changeOpacity(int opacity)
{
    if(m_initializing)
        return;
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoCommand *cmd = new ChangeOpacityCommand(opacity, m_item->opacity(), as, m_item);
        as->undoStack()->push(cmd);
    }
}

SvgAttributeEditor *ItemPropertyEditor::addSvgAttributeEditor(QString name, QString value)
{
    SvgAttributeEditor *attributeEditor = new SvgAttributeEditor;
    attributeEditor->setAttributeName(name);
    attributeEditor->setValue(value);
    connect(attributeEditor, SIGNAL(attributeNameChanged(QString,QString)), this, SLOT(svgAttributeNameChanged(QString,QString)));
    connect(attributeEditor, SIGNAL(attributeValueChanged(QString,QString)), this, SLOT(svgAttributeValueChanged(QString,QString)));
    connect(attributeEditor, SIGNAL(removeClicked(SvgAttributeEditor*)), this, SLOT(svgEditorRemoveClicked(SvgAttributeEditor*)));
    connect(attributeEditor, SIGNAL(addKeyframeClicked(SvgAttributeEditor*)), this, SLOT(svgEditorAddKeyframeClicked(SvgAttributeEditor*)));

    m_vboxAttributeEditors->addWidget(attributeEditor);
    return attributeEditor;
}

void ItemPropertyEditor::opacityChanged(int val)
{
    m_initializing = true;
    m_opacity->setValue(val);
    m_opacityText->setValue(val);
    m_initializing = false;
}
