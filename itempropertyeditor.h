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

#ifndef ITEMPROPERTYEDITOR_H
#define ITEMPROPERTYEDITOR_H

#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QFontDatabase>
#include <QComboBox>

class ResizeableItem;
class Text;
class ColorEditor;
class Expander;
class Rectangle;
class Ellipse;
class Vectorgraphic;
class QTextEdit;
class QVBoxLayout;

class SvgAttributeEditor : public QWidget
{
    Q_OBJECT
public:
    SvgAttributeEditor();

    bool isValid() {return !m_element->text().isEmpty() && !m_attribute->text().isEmpty();}
    QString attributeName() {return m_element->text() + "." + m_attribute->text();}
    void setAttributeName(QString name);
    int value() {return m_value->value();}
    void setValue(int val) {m_value->setValue(val);}

signals:
    void attributeNameChanged(QString oldName, QString newName);
    void attributeValueChanged(QString name, int value);
    void removeClicked(SvgAttributeEditor *editor);
    void addKeyframeClicked(SvgAttributeEditor *editor);

private slots:
    void attributeNameChanged();
    void valueChanged(int value);
    void minusClicked();
    void addKeyframeClicked();

private:
    QLineEdit *m_element;
    QLineEdit *m_attribute;
    QSpinBox *m_value;
    QString m_attributeName;
};

class ItemPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    ItemPropertyEditor();

    void setItem(ResizeableItem *item);

signals:
    void addKeyFrame(ResizeableItem*, QString, QVariant);

private:
    bool m_initializing;
    QFontDatabase m_fontdatabase;
    ColorEditor *colorEditor;
    ColorEditor *borderColorEditor;
    ColorEditor *textcolorEditor;
    Rectangle *m_rectangle;
    Vectorgraphic *m_vector;
    QTextEdit *m_svgText;
    QComboBox *m_font;
    QComboBox *m_fontSize;
    QComboBox *m_style;
    QLineEdit *m_id;
    QSpinBox *m_x;
    QSpinBox *m_y;
    QSpinBox *m_width;
    QSpinBox *m_height;
    QSpinBox *m_opacityText;
    QLabel *m_typ;
    QLineEdit *m_text;
    Expander *expText;
    Expander *expTextcolor;
    Expander *expColor;
    Expander *expSvg;
    ResizeableItem *m_item;
    Text *m_textitem;

    Ellipse *m_ellipse;
    QSlider *m_opacity;
    QVBoxLayout *m_vboxAttributeEditors;

    void changeBrush(QColor value);
    void changeOpacity(int opacity);
    SvgAttributeEditor *addSvgAttributeEditor(QString name, int value);

private slots:
    void idChanged();
    void idChanged(ResizeableItem*,QString);
    void brushChanged(QColor color);
    void penChanged(QColor color);
    void textcolorChanged(QColor color);
    void xChanged(int value);
    void yChanged(int value);
    void widthChanged(int value);
    void heightChanged(int value);
    void textChanged(QString value);
    void addLeftKeyFrame();
    void addTopKeyFrame();
    void addWidthKeyFrame();
    void addHeightKeyFrame();
    void addOpacityKeyFrame();
    void itemSizeChanged(qreal width, qreal height);
    void itemPositionChanged(qreal x, qreal y);
    void opacitySliderReleased();
    void opacityTextChanged(int value);
    void opacityChanged(int value);
    void colorChanged(QColor);
    void borderColorChanged(QColor);
    void textColorChanged(QColor);
    void fontFamilyChanged(int index);
    void fontStyleChanged(int index);
    void fontSizeChanged();
    void svgAttributeNameChanged(QString oldName, QString newName);
    void svgAttributeValueChanged(QString attributeName, int value);
    void svgTextChanged();
    void addSvgAttributeEditor();
    void svgEditorRemoveClicked(SvgAttributeEditor *editor);
    void svgEditorAddKeyframeClicked(SvgAttributeEditor *editor);
 };

#endif // ITEMPROPERTYEDITOR_H
