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
#include <QGraphicsSceneMouseEvent>
#include <QDataStream>
#include <QGraphicsItem>
#include <QTest>
#include <QFileDialog>

#include "resizeableitem.h"

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

    inline int length() const {return m_length;}
    inline void setLength(int value) {m_length = value;}

    inline QColor backgroundColor() const {return m_backgroundColor;}
    inline void setBackgroundColor(QColor value) {m_backgroundColor = value; setBackgroundBrush(QBrush(QColor(m_backgroundColor)));}

    void copyItem();
    void pasteItem();

signals:
    void itemAdded(QGraphicsItem *item);
    void addPropertyAnimation(ResizeableItem *item, const QString propertyName, qreal value, int min, int max);
    void animationAdded(ResizeableItem *item, QPropertyAnimation *anim);

public slots:
    void addPropertyAnimationRequested(ResizeableItem *item, const QString propertyName, qreal value, int min, int max);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

private:
    EditMode m_editMode;
    int m_fps;
    int m_length;
    ResizeableItem *m_copy;
    QColor m_backgroundColor;

    void initialize();
};

QDataStream &operator<<(QDataStream &, const AnimationScene *);
QDataStream &operator>>(QDataStream &, AnimationScene *);
QString getItemName(ResizeableItem *item);
QString getItemTypeName(ResizeableItem *item);
bool isAnimationMakerItem(QGraphicsItem *item);

#endif // ANIMATIONSCENE_H
