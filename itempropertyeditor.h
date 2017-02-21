#ifndef ITEMPROPERTYEDITOR_H
#define ITEMPROPERTYEDITOR_H

#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include "resizeableitem.h"
#include "expander.h"
#include "text.h"

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
    ResizeableItem *m_item;
    Text *m_textitem;

private slots:
    void xChanged(int value);
    void yChanged(int value);
    void widthChanged(int value);
    void heightChanged(int value);
    void textChanged(QString value);
};

#endif // ITEMPROPERTYEDITOR_H
