/****************************************************************************
** Copyright (C) 2018 Olaf Japp
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

#ifndef KEYFRAMEHANDLE_H
#define KEYFRAMEHANDLE_H

#include <QWidget>

#include "widgets_global.h"

class QMenu;
class TransitionLine;
class KeyFrame;
class AnimationItem;
class WIDGETSSHARED_EXPORT KeyframeHandle : public QWidget
{
    Q_OBJECT
public:
    KeyframeHandle(TransitionLine *parent, KeyFrame *key);

    void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;

    KeyFrame *key() {return m_key;}

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

private slots:
    void onCustomContextMenu(const QPoint &point);
    void addTransition();
    void deleteKeyframe();

signals:
    void transitionAdded(KeyFrame *key);
    void keyframeDeleted(KeyframeHandle *handle);
    void keyframeMoved(KeyframeHandle *handle, int dist);

private:
    QImage m_imageRaute;
    QImage m_imageRauteHohl;
    QMenu *m_contextMenu;
    QAction *m_transitionAct;
    QAction *m_delKeyframeAct;
    KeyFrame *m_key;
    bool m_pressed;
};

#endif // KEYFRAMEHANDLE_H
