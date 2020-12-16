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

class AnimationItem;
class Text;
class ColorEditor;
class Expander;
class Rectangle;
class Ellipse;
class Vectorgraphic;
class Timeline;
class QVBoxLayout;
class XmlEditor;
class SvgAttributeEditor;
class QRadioButton;
class ItemPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    ItemPropertyEditor(Timeline *timeline);

    void setItem(AnimationItem *item);

signals:
    void addKeyFrame(AnimationItem*, QString, QVariant);

private:
    bool m_initializing;
    Timeline *m_timeline;
    QFontDatabase m_fontdatabase;
    ColorEditor *colorEditor;
    ColorEditor *borderColorEditor;
    ColorEditor *textcolorEditor;
    Rectangle *m_rectangle;
    Vectorgraphic *m_vector;
    XmlEditor *m_svgText;
    QComboBox *m_font;
    QComboBox *m_fontSize;
    QComboBox *m_style;
    QLineEdit *m_id;
    QSpinBox *m_x;
    QSpinBox *m_y;
    QSpinBox *m_width;
    QSpinBox *m_height;
    QSpinBox *m_opacityText;
    QSpinBox *m_scaleX;
    QSpinBox *m_scaleY;
    QSpinBox *m_shearX;
    QSpinBox *m_shearY;
    QSpinBox *m_rotateX;
    QSpinBox *m_rotateY;
    QSpinBox *m_rotateZ;
    QSpinBox *m_origY;
    QSpinBox *m_origX;
    QLabel *m_typ;
    QLineEdit *m_text;
    Expander *expText;
    Expander *expTextcolor;
    Expander *expColor;
    Expander *expSvg;
    AnimationItem *m_item;
    Text *m_textitem;
    QVBoxLayout *m_additionalPropertyBox;
    Ellipse *m_ellipse;
    QSlider *m_opacity;
    QVBoxLayout *m_vboxAttributeEditors;

    void changeBrush(QColor value);
    void changeOpacity(int opacity);
    SvgAttributeEditor *addSvgAttributeEditor(QString name, QString value);

    void reloadAttributes();

private slots:
    void idChanged();
    void idChanged(AnimationItem*,QString);
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
    void svgAttributeValueChanged(QString attributeName, QString value);
    void svgTextChanged();
    void addSvgAttributeEditor();
    void svgEditorRemoveClicked(SvgAttributeEditor *editor);
    void svgEditorAddKeyframeClicked(SvgAttributeEditor *editor);
    void svgAttributeAdded();
    void svgDataChanged();
    void addBrushKeyFrame();
    void addPenKeyFrame();
    void addTextColorKeyFrame();
    void scaleXChanged(int);
    void scaleYChanged(int);
    void shearXChanged(int);
    void shearYChanged(int);
    void rotationXChanged(int);
    void rotationYChanged(int);
    void rotationZChanged(int);
    void addScaleXKeyframe();
    void addScaleYKeyframe();
    void addShearXKeyframe();
    void addShearYKeyframe();
    void addRotateXKeyframe();
    void addRotateYKeyframe();
    void addRotateZKeyframe();
    void origXChanged(int);
    void origYChanged(int);
 };

#endif // ITEMPROPERTYEDITOR_H
