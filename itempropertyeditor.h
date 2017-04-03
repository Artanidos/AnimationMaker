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
#include "resizeableitem.h"
#include "expander.h"
#include "text.h"
#include "rectangle.h"
#include "ellipse.h"
#include "colorpicker.h"
#include "colorrect.h"

class ItemPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    ItemPropertyEditor();

    void setItem(ResizeableItem *item);

signals:
    void addKeyFrame(ResizeableItem*, QString, qreal);

private:
    bool m_initializing;
    QLineEdit *m_id;
    QSpinBox *m_x;
    QSpinBox *m_y;
    QSpinBox *m_width;
    QSpinBox *m_height;
    QSpinBox *m_opacityText;
    QSpinBox *m_hue;
    QSpinBox *m_saturation;
    QSpinBox *m_lightness;
    QSpinBox *m_red;
    QSpinBox *m_green;
    QSpinBox *m_blue;
    QLabel *m_labelHue;
    QLabel *m_labelSaturation;
    QLabel *m_labelLightness;
    QLabel *m_labelRed;
    QLabel *m_labelGreen;
    QLabel *m_labelBlue;
    QSpinBox *m_hueBorder;
    QSpinBox *m_saturationBorder;
    QSpinBox *m_lightnessBorder;
    QSpinBox *m_redBorder;
    QSpinBox *m_greenBorder;
    QSpinBox *m_blueBorder;
    QLabel *m_labelHueBorder;
    QLabel *m_labelSaturationBorder;
    QLabel *m_labelLightnessBorder;
    QLabel *m_labelRedBorder;
    QLabel *m_labelGreenBorder;
    QLabel *m_labelBlueBorder;
    QSpinBox *m_hueText;
    QSpinBox *m_saturationText;
    QSpinBox *m_lightnessText;
    QSpinBox *m_redText;
    QSpinBox *m_greenText;
    QSpinBox *m_blueText;
    QLabel *m_labelHueText;
    QLabel *m_labelSaturationText;
    QLabel *m_labelLightnessText;
    QLabel *m_labelRedText;
    QLabel *m_labelGreenText;
    QLabel *m_labelBlueText;
    QLabel *m_typ;
    QLineEdit *m_text;
    Expander *expText;
    Expander *expTextcolor;
    Expander *expColor;
    ResizeableItem *m_item;
    Text *m_textitem;
    Rectangle *m_rectangle;
    Ellipse *m_ellipse;
    QLineEdit *m_textcolor;
    QLineEdit *m_brushcolor;
    QLineEdit *m_pencolor;
    ColorPicker *m_colorPicker;
    ColorPicker *m_bordercolorPicker;
    ColorPicker *m_textcolorpicker;
    QSlider *m_opacity;
    ColorRect *m_colorRect;
    ColorRect *m_borderColorRect;
    ColorRect *m_textcolorRect;
    QSlider *m_hueColorSlider;
    QSlider *m_hueBordercolorSlider;
    QSlider *m_hueTextColorSlider;

    void setColorParts(QColor value);
    void setBordercolorParts(QColor value);
    void setTextcolorParts(QColor value);

private slots:
    void idChanged(QString value);
    void xChanged(int value);
    void yChanged(int value);
    void widthChanged(int value);
    void heightChanged(int value);
    void textChanged(QString value);
    void textcolorChanged(QString value);
    void textcolorChanged(QColor value);
    void colorChanged(QString value);
    void colorChanged(QColor value);
    void borderColorChanged(QString);
    void bordercolorChanged(QColor value);
    void hueChanged(int);
    void hueBorderChanged(int);
    void hueTextcolorChanged(int);
    void addLeftKeyFrame();
    void addTopKeyFrame();
    void addWidthKeyFrame();
    void addHeightKeyFrame();
    void addOpacityKeyFrame();
    void itemSizeChanged(qreal width, qreal height);
    void itemPositionChanged(qreal x, qreal y);
    void opacityChanged(int value);
    void opacityTextChanged(int value);
    void colorRectClicked();
    void borderColorRectClicked();
    void textColorRectClicked();
    void hueValueChanged(int value);
    void saturationValueChanged(int value);
    void lightnessValueChanged(int value);
    void redValueChanged(int value);
    void greenValueChanged(int value);
    void blueValueChanged(int value);
    void hueBorderValueChanged(int value);
    void saturationBorderValueChanged(int value);
    void lightnessBorderValueChanged(int value);
    void redBorderValueChanged(int value);
    void greenBorderValueChanged(int value);
    void blueBorderValueChanged(int value);
    void hueTextValueChanged(int value);
    void saturationTextValueChanged(int value);
    void lightnessTextValueChanged(int value);
    void redTextValueChanged(int value);
    void greenTextValueChanged(int value);
    void blueTextValueChanged(int value);
};

#endif // ITEMPROPERTYEDITOR_H
