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

#ifndef PLAYHEAD_H
#define PLAYHEAD_H

#include <QWidget>

class PlayHead : public QWidget
{
    Q_OBJECT
public:
    PlayHead();

    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

    inline void setValue(int value) {m_value = value; update(); emit valueChanged(value);}
    inline int value() {return m_value;}

public slots:
    void scrollValueChanged(int pos);

signals:
    void valueChanged(int val);

private:
    QImage m_image;
    bool m_pressed;
    int m_value;
    int m_horizontalScrollPos;
};

#endif // PLAYHEAD_H
