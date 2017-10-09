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
#include "resizeableitem.h"
#include "animationscene.h"
#include "xmlhighlighter.h"
#include "text.h"
#include "rectangle.h"
#include "ellipse.h"
#include "flatbutton.h"
#include "expander.h"
#include "coloreditor.h"
#include "vectorgraphic.h"
#include "commands.h"
#include <QPushButton>
#include <QComboBox>
#include <QFontDatabase>
#include <QTextEdit>

XmlEditor::XmlEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(13);

    setFont(font);
    setAcceptRichText(false);
    setLineWrapMode(QTextEdit::NoWrap);
    setMinimumHeight(200);
    QFontMetrics metrics(font);
    setTabStopWidth(4 * metrics.width(' '));
    new XmlHighlighter(document());
}

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

ItemPropertyEditor::ItemPropertyEditor()
{
    m_rectangle = NULL;
    m_ellipse = NULL;
    m_vector = NULL;
    m_initializing = false;

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
    vbox->addStretch();

    QWidget *scrollContent = new QWidget();
    scrollContent->setLayout(vbox);
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
    connect(borderColorEditor, SIGNAL(colorChanged(QColor)), this, SLOT(borderColorChanged(QColor)));
    connect(textcolorEditor, SIGNAL(colorChanged(QColor)), this, SLOT(textColorChanged(QColor)));
    connect(m_font, SIGNAL(currentIndexChanged(int)), this, SLOT(fontFamilyChanged(int)));
    connect(m_style, SIGNAL(currentIndexChanged(int)), this, SLOT(fontStyleChanged(int)));
    connect(m_fontSize, SIGNAL(currentTextChanged(QString)), this, SLOT(fontSizeChanged()));
    connect(m_svgText, SIGNAL(editingFinished()), this, SLOT(svgTextChanged()));
    connect(plus, SIGNAL(clicked()), this, SLOT(addSvgAttributeEditor()));
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
        QUndoCommand *cmd = new ChangeColorCommand(color, m_item->brush().color(), m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::borderColorChanged(QColor color)
{
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangePenCommand(color, m_item->pen().color(), m_item);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::textColorChanged(QColor color)
{
    AnimationScene *as = dynamic_cast<AnimationScene *>(m_item->scene());
    if(as)
    {
        QUndoStack *undoStack = as->undoStack();
        QUndoCommand *cmd = new ChangeTextcolorCommand(color, m_textitem->textcolor(), m_textitem);
        undoStack->push(cmd);
    }
}

void ItemPropertyEditor::addLeftKeyFrame()
{
    emit addKeyFrame(m_item, "left", QVariant(m_x->value()));
}

void ItemPropertyEditor::addTopKeyFrame()
{
    emit addKeyFrame(m_item, "top", QVariant(m_y->value()));
}

void ItemPropertyEditor::addWidthKeyFrame()
{
    emit addKeyFrame(m_item, "width", QVariant(m_width->value()));
}

void ItemPropertyEditor::addHeightKeyFrame()
{
    emit addKeyFrame(m_item, "height", QVariant(m_height->value()));
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

void ItemPropertyEditor::setItem(ResizeableItem *item)
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

    m_textitem = dynamic_cast<Text*>(item);
    if(m_textitem)
    {
        m_text->setText(m_textitem->text());
        textcolorEditor->setColor(m_textitem->textcolor());
        QFont f = m_textitem->font();
        m_font->setCurrentText(f.family());
        m_style->setCurrentText(f.styleName());
        m_fontSize->setCurrentText(QString::number(f.pointSize()));
        connect(m_textitem, SIGNAL(textcolorChanged(QColor)), this, SLOT(textcolorChanged(QColor)));
    }

    m_rectangle = dynamic_cast<Rectangle*>(item);
    if(m_rectangle)
    {
        expColor->setVisible(true);
        colorEditor->setColor(m_rectangle->brush().color());
        borderColorEditor->setColor(m_rectangle->pen().color());
    }

    m_ellipse = dynamic_cast<Ellipse*>(item);
    if(m_ellipse)
    {
        expColor->setVisible(true);
        colorEditor->setColor(m_ellipse->brush().color());
        borderColorEditor->setColor(m_ellipse->pen().color());
    }

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

    connect(item, SIGNAL(idChanged(ResizeableItem*,QString)), this, SLOT(idChanged(ResizeableItem*,QString)));
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

void ItemPropertyEditor::idChanged(ResizeableItem*, QString id)
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
        QUndoCommand *cmd = new ChangeColorCommand(color, m_item->brush().color(), m_item);
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
