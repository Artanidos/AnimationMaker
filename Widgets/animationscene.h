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
#include <QUndoStack>
#include <QDomDocument>
#include "widgets_global.h"

class AnimationItem;
class KeyFrame;
class WIDGETSSHARED_EXPORT AnimationScene : public QGraphicsScene
{
    Q_OBJECT
    Q_PROPERTY(int fps READ fps)
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)

public:
    AnimationScene();

    void reset();

    enum ItemType { TypeItem, TypeRectangle, TypeEllipse, TypeText, TypeBitmap, TypeSvg };
    enum EditMode { ModeSelect, ModeRectangle, ModeEllipse, ModeText, ModeBitmap, ModeSvg, ModePlugin };

    void setEditMode(EditMode mode);
    void setEditMode(QString pluginName);
    QDataStream& read(QDataStream &dataStream);

    void setFileVersion(int version) {m_fileVersion = version;}

    int fps() const;
    inline void setFps(int value) {m_fps = value;}

    inline void setWidth(int value) {setSceneRect(0, 0, value, height()); m_rect->setRect(0,0,value, height()); emit sizeChanged(value, height());}
    inline void setHeight(int value) {setSceneRect(0, 0, width(), value); m_rect->setRect(0,0,width(), value); emit sizeChanged(width(), value);}

    inline QColor backgroundColor() const {return m_backgroundColor;}
    inline void setBackgroundColor(QColor value)
    {
        m_backgroundColor = value;
        m_rect->setBrush(QBrush(QColor(m_backgroundColor)));
        emit backgroundColorChanged(value);
    }

    inline int playheadPosition() {return m_playheadPosition;}

    void setCursor(const QCursor &cursor) {m_rect->setCursor(cursor);}
    void copyItem();
    void pasteItem();

    void deleteItem(AnimationItem *item);
    inline bool autokeyframes() {return m_autokeyframes;}
    inline void setAutokeyframes(bool value) {m_autokeyframes = value;}

    inline bool autotransition() {return m_autotransitions;}
    inline void setAutotransition(bool value) {m_autotransitions = value;}

    inline void registerUndoStack(QUndoStack *stack) {m_undoStack = stack;}
    inline QUndoStack *undoStack() {return m_undoStack;}

    QString actPluginName() {return m_actPluginName;}

    void exportXml(QString fileName, bool exportAll = true);
    bool importXml(QString fileName);
        
signals:
    void itemAdded(QGraphicsItem *item);
    void sizeChanged(int width, int height);
    void keyframeAdded(AnimationItem *item, QString propertyName, KeyFrame *key);
    void itemRemoved(AnimationItem *item);
    void backgroundColorChanged(QColor color);
    void animationResetted();

public slots:
    void setPlayheadPosition(int value);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

private:
    EditMode m_editMode;
    int m_fps;
    AnimationItem *m_copy;
    QColor m_backgroundColor;
    QGraphicsRectItem *m_rect;
    int m_playheadPosition;
    bool m_autokeyframes;
    bool m_autotransitions;
    QUndoStack *m_undoStack;
    AnimationItem *m_movingItem;
    QPointF m_oldPos;
    int m_fileVersion;
    QString m_actPluginName;

    void initialize();
    void addBackgroundRect();
    void readKeyframes(QDataStream &dataStream, AnimationItem *item);
    void copyKeyframes(AnimationItem *item);
    void writeKeyframes(QDomDocument *doc, QDomElement *element, AnimationItem *item);
    void readKeyframes(QDomElement *element, AnimationItem *item);
};

qreal getProgressValue(KeyFrame *found, int playheadPosition);
qreal calculateRealValue(KeyFrame *found, int playheadPosition);
QColor calculateColorValue(KeyFrame *found, int playheadPosition);
int calculateIntValue(KeyFrame *found, int playheadPosition);
QDataStream &operator>>(QDataStream &, AnimationScene *);

#endif // ANIMATIONSCENE_H
