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

class ItemPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    ItemPropertyEditor();

    void setItem(ResizeableItem *item);

signals:
    void addKeyFrame(ResizeableItem*, QString, qreal);

private:
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
    ResizeableItem *m_item;
    Text *m_textitem;
    Rectangle *m_rectangle;
    Ellipse *m_ellipse;
    QLineEdit *m_textcolor;
    QLineEdit *m_brushcolor;
    QLineEdit *m_pencolor;
    ColorPicker *m_colorpicker;
    ColorPicker *m_textcolorpicker;
    QSlider *m_opacity;

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
    void hueChanged(int);
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
};

#endif // ITEMPROPERTYEDITOR_H
