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

#ifndef TRANSITION_H
#define TRANSITION_H

#include <QWidget>
#include "resizeableitem.h"

class TransitionLine : public QWidget
{
    Q_OBJECT
public:
    TransitionLine(ResizeableItem *item, QString propertyName);

    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

    inline QString propertyName() {return m_propertyName;}
    inline ResizeableItem *item() {return m_item;}
    inline void setPlayheadPosition(int value) {m_playheadPosition = value; update();}
    inline void setScrollValue(int value) {m_horizontalScrollValue = value; update();}
    inline KeyFrame *selectedFrame() {return m_selectedFrame;}
    inline void deselectFrame() {m_selectedFrame = NULL; update();}

private slots:
    void onCustomContextMenu(const QPoint &point);
    void addTransition();
    void deleteKeyframe();
    void deleteTransition();

signals:
    void deleteKeyframe(ResizeableItem *item, QString propertyName, KeyFrame *frame);
    void deleteTransition(ResizeableItem *item, QString propertyName, KeyFrame *frame);
    void addTransition(ResizeableItem *item, QString propertyName, KeyFrame *frame);
    void transitionSelected(KeyFrame *frame);

private:
    QImage m_imageRaute;
    QImage m_imageRauteHohl;
    QImage m_imageLeft;
    QImage m_imageRight;
    ResizeableItem *m_item;
    QString m_propertyName;
    KeyFrame *m_frame;
    KeyFrame *m_selectedFrame;
    int m_oldx;
    int m_playheadPosition;
    QMenu *m_contextMenu;
    QAction *m_transitionAct;
    QAction *m_delKeyframeAct;
    QAction *m_delTransitionAct;
    bool m_pressed;
    int m_horizontalScrollValue;
};

#endif // TRANSITION_H
