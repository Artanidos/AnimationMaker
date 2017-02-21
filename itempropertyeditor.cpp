#include "itempropertyeditor.h"
#include "animationscene.h"

ItemPropertyEditor::ItemPropertyEditor()
{
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
    m_x->setMaximum(10000);
    m_y->setMaximum(10000);
    m_width->setMaximum(10000);
    m_height->setMaximum(10000);
    layoutGeo->addWidget(labelPosition, 0, 0);
    layoutGeo->addWidget(labelX, 0, 1);
    layoutGeo->addWidget(m_x, 0, 2);
    layoutGeo->addWidget(labelY, 0, 3);
    layoutGeo->addWidget(m_y, 0, 4);

    layoutGeo->addWidget(labelSize, 1, 0);
    layoutGeo->addWidget(labelWidth, 1, 1);
    layoutGeo->addWidget(m_width, 1, 2);
    layoutGeo->addWidget(labelHeight, 1, 3);
    layoutGeo->addWidget(m_height, 1, 4);
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
    vbox->addStretch();
    this->setLayout(vbox);

    connect(m_x, SIGNAL(valueChanged(int)), this, SLOT(xChanged(int)));
    connect(m_y, SIGNAL(valueChanged(int)), this, SLOT(yChanged(int)));
    connect(m_width, SIGNAL(valueChanged(int)), this, SLOT(widthChanged(int)));
    connect(m_height, SIGNAL(valueChanged(int)), this, SLOT(heightChanged(int)));
    connect(m_text, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
}

void ItemPropertyEditor::setItem(ResizeableItem *item)
{
    m_item = item;
    m_x->setValue(m_item->x());
    m_y->setValue(m_item->y());
    m_width->setValue(m_item->rect().width());
    m_height->setValue(m_item->rect().height());
    m_typ->setText(getItemTypeName(m_item));

    m_textitem = dynamic_cast<Text*>(item);
    if(m_textitem)
    {
        m_text->setText(m_textitem->text());
    }
    expText->setVisible(m_textitem);
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
}

void ItemPropertyEditor::heightChanged(int value)
{
    m_item->setHeight(value);
}

void ItemPropertyEditor::textChanged(QString value)
{
    m_textitem->setText(value);
}
