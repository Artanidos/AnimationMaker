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

#ifndef EXPANDER_H
#define EXPANDER_H

#include <QLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QScrollArea>

class Expander : public QWidget
{
    Q_OBJECT
public:
    Expander(QString header);

    void addLayout(QLayout *l);

public slots:
    void buttonClicked(bool);

private :
    QWidget *m_content;
    QToolButton *m_button;
    bool m_isExpanded;
};

#endif // EXPANDER_H
