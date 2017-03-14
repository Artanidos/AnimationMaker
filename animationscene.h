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

#ifndef ANIMATIONSCENE_H
#define ANIMATIONSCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QDataStream>
#include <QGraphicsItem>
#include <QTest>
#include <QFileDialog>

#include "resizeableitem.h"
#include "keyframe.h"

class AnimationScene : public QGraphicsScene
{
    Q_OBJECT
public:
    AnimationScene();

    void reset();

    enum ItemType { TypeItem, TypeRectangle, TypeEllipse, TypeText, TypeBitmap, TypeSvg };
    enum EditMode { ModeSelect, ModeRectangle, ModeEllipse, ModeText, ModeBitmap, ModeSvg };

    void setEditMode(EditMode mode);
    QDataStream& read(QDataStream &dataStream);
    QDataStream& write(QDataStream &dataStream) const;

    inline int fps() const {return m_fps;}
    inline void setFps(int value) {m_fps = value;}

    inline void setWidth(int value) {setSceneRect(0, 0, value, height()); m_rect->setRect(0,0,value, height()); emit sizeChanged(value, height());}
    inline void setHeight(int value) {setSceneRect(0, 0, width(), value); m_rect->setRect(0,0,width(), value); emit sizeChanged(width(), value);}

    inline QColor backgroundColor() const {return m_backgroundColor;}
    inline void setBackgroundColor(QColor value) {m_backgroundColor = value; m_rect->setBrush(QBrush(QColor(m_backgroundColor)));}

    inline int playheadPosition() {return m_playheadPosition;}

    void copyItem();
    void pasteItem();
        
signals:
    void itemAdded(QGraphicsItem *item);
    void sizeChanged(int width, int height);
    void keyframeAdded(ResizeableItem *item, QString propertyName, KeyFrame *key);

public slots:
    void setPlayheadPosition(int value);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

private:
    EditMode m_editMode;
    int m_fps;
    ResizeableItem *m_copy;
    QColor m_backgroundColor;
    QGraphicsRectItem *m_rect;
    int m_playheadPosition;

    void initialize();
    void addBackgroundRect();
    void writeKeyframes(QDataStream &dataStream, ResizeableItem *item) const;
    void readKeyframes(QDataStream &dataStream, ResizeableItem *item);

};

QDataStream &operator<<(QDataStream &, const AnimationScene *);
QDataStream &operator>>(QDataStream &, AnimationScene *);
QString getItemTypeName(ResizeableItem *item);
bool isAnimationMakerItem(QGraphicsItem *item);

#endif // ANIMATIONSCENE_H
