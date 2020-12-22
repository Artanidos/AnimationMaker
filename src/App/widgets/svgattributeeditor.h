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

#ifndef SVGATTRIBUTEEDITOR_H
#define SVGATTRIBUTEEDITOR_H

#include <QLineEdit>
#include <QString>
#include <QWidget>

class SvgAttributeEditor : public QWidget
{
    Q_OBJECT
public:
    SvgAttributeEditor();

    bool isValid() {return !m_element->text().isEmpty() && !m_attribute->text().isEmpty();}
    QString attributeName() {return m_element->text() + "." + m_attribute->text();}
    void setAttributeName(QString name);
    QString value() {return m_value->text();}
    void setValue(QString val) {m_value->setText(val);}

signals:
    void attributeNameChanged(QString oldName, QString newName);
    void attributeValueChanged(QString name, QString value);
    void removeClicked(SvgAttributeEditor *editor);
    void addKeyframeClicked(SvgAttributeEditor *editor);

private slots:
    void attributeNameChanged();
    void valueChanged();
    void minusClicked();
    void addKeyframeClicked();

private:
    QLineEdit *m_element;
    QLineEdit *m_attribute;
    QLineEdit *m_value;
    QString m_attributeName;
};

#endif // SVGATTRIBUTEEDITOR_H
