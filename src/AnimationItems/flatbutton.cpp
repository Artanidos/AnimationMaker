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

#include "flatbutton.h"

#include <QImage>

FlatButton::FlatButton()
{
    m_enabled = true;
    m_returncode = "";
    setCursor(Qt::PointingHandCursor);
}

FlatButton::FlatButton(QString normalIcon, QString hoverIcon, QString pressedIcon, QString disabledIcon)
{
    m_enabled = true;
    m_returncode = "";
    if(normalIcon.isEmpty())
        m_normalIcon = QPixmap::fromImage(QImage());
    else
        m_normalIcon = QPixmap::fromImage(QImage(normalIcon));
    if(hoverIcon.isEmpty())
        m_hoverIcon = QPixmap::fromImage(QImage());
    else
        m_hoverIcon = QPixmap::fromImage(QImage(hoverIcon));
    if(pressedIcon.isEmpty())
        m_pressedIcon = QPixmap::fromImage(QImage(hoverIcon));
    else
        m_pressedIcon = QPixmap::fromImage(QImage(pressedIcon));
    if(disabledIcon.isEmpty())
        m_disabledIcon = QPixmap::fromImage(QImage(normalIcon));
    else
        m_disabledIcon = QPixmap::fromImage(QImage(disabledIcon));
    setPixmap(m_normalIcon);
    setCursor(Qt::PointingHandCursor);
}

void FlatButton::setEnabled(bool enabled)
{
    m_enabled = enabled;
    if(enabled)
        setPixmap(m_normalIcon);
    else
        setPixmap(m_disabledIcon);

}

void FlatButton::mousePressEvent(QMouseEvent *event)
{
    if(m_enabled)
        setPixmap(m_pressedIcon);
    setFocus();
    event->accept();
}

void FlatButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_enabled)
        setPixmap(m_hoverIcon);
    event->accept();
    if(m_returncode.isEmpty())
        emit clicked();
    else
        emit clicked(m_returncode);
}

void FlatButton::enterEvent(QEvent * event)
{
    if(m_enabled)
        setPixmap(m_hoverIcon);
    QWidget::enterEvent(event);
}

void FlatButton::leaveEvent(QEvent * event)
{
    if(m_enabled)
        setPixmap(m_normalIcon);
    QWidget::leaveEvent(event);
}
