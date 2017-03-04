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

private:
    QLineEdit *m_id;
    QSpinBox *m_x;
    QSpinBox *m_y;
    QSpinBox *m_width;
    QSpinBox *m_height;
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
};

#endif // ITEMPROPERTYEDITOR_H
