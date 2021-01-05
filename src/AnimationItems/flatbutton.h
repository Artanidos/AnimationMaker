/****************************************************************************
** Copyright (C) 2017 Olaf Japp
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

#ifndef FLATBUTTON_H
#define FLATBUTTON_H

#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QString>

class  FlatButton : public QLabel
{
    Q_OBJECT

public:
    FlatButton();
    FlatButton(QString normal, QString hover, QString pressed = "", QString disabled = "");

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
    void setEnabled(bool enabled);
    void setReturnCode(QString rc) {m_returncode = rc;}
    void setNormalPixmap(QPixmap pm)
    {
        m_normalIcon = pm;
        if(m_disabledIcon.isNull())
            m_disabledIcon = pm;
        if(m_pressedIcon.isNull())
            m_pressedIcon = pm;
        if(m_hoverIcon.isNull())
            m_hoverIcon = pm;
        setPixmap(m_normalIcon);
    }
    void setHoverPixmap(QPixmap pm)
    {
        m_hoverIcon = pm;
        if(m_pressedIcon.isNull())
            m_pressedIcon = pm;
    }

signals:
    void clicked();
    void clicked(QString rc);

private:
    QPixmap m_normalIcon;
    QPixmap m_hoverIcon;
    QPixmap m_pressedIcon;
    QPixmap m_disabledIcon;
    bool m_enabled;
    QString m_returncode;
};

#endif // FLATBUTTON_H
