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

#ifndef RESIZEABLEITEM_H
#define RESIZEABLEITEM_H

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QAction>
#include <QList>
#include <QDomElement>
#include <QTest>
#include "itemhandle.h"
#include "keyframe.h"
#include "widgets_global.h"

class AnimationScene;
class PropertyEditorInterface;
class WIDGETSSHARED_EXPORT AnimationItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(qreal left READ left WRITE setX)
    Q_PROPERTY(qreal top READ top WRITE setY)
    Q_PROPERTY(qreal width READ top WRITE setWidth)
    Q_PROPERTY(qreal height READ top WRITE setHeight)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QColor brushColor READ brushColor WRITE setBrushColor)
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)
    Q_INTERFACES(QGraphicsItem)

public:
    AnimationItem(AnimationScene *scene, bool isSceneRect = false);
    ~AnimationItem();

    virtual QString typeName() = 0;
    virtual QDomElement getXml(QDomDocument) = 0;
    virtual bool hasBrushAndPen() = 0;
    virtual QList<PropertyEditorInterface*> *getPropertyEditors() {return NULL;}

    void setPlayheadPosition(int pos) {m_playheadPosition = pos; update();}
    int playheadPosition() {return m_playheadPosition;}
    void setScene(AnimationScene *scene);
    void drawHighlightSelected(QPainter *painter, const QStyleOptionGraphicsItem *option);
    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *);
    QRectF boundingRect() const;
    bool sceneEventFilter(QGraphicsItem * watched, QEvent * event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void scaleObjects();
    virtual void setScale(qreal x, qreal y);
    qreal xscale();
    qreal yscale();

    QString id() const;
    void setId(const QString value);

    QRectF rect() const;
    void setRect(qreal x, qreal y, qreal w, qreal h);

    QPen pen() const;
    void setPen(const QPen &pen);

    QBrush brush() const;
    void setBrush(const QBrush &brush);

    QColor brushColor() const;
    void setBrushColor(const QColor &color);

    QColor penColor() const;
    void setPenColor(const QColor &color);

    inline int opacity() {return m_opacity;}
    void setOpacity(int opacity);
    inline qreal left() {return x();}
    inline qreal top() {return y();}
    inline void setLeft(qreal val) {setX(val);}
    inline void setTop(qreal val) {setY(val);}
    inline void setDeleted(bool val) {m_deleted = val;}
    inline bool isDeleted() {return m_deleted;}

    void setWidth(qreal value);
    void setHeight(qreal value);
    qreal width() {return rect().width();}
    qreal height() {return rect().height();}

    void addKeyframe(QString propertyName, KeyFrame *frame);
    bool deleteKeyframe(QString propertyName, KeyFrame *frame);
    inline QHash<QString, KeyFrame*> *keyframes() {return m_keyframes;}

    void adjustKeyframes(QString propertyName, QVariant value, int time, bool autokeyframes, bool autotransition);

    void posChanged(qreal x, qreal y);

    inline AnimationScene *scene() {return m_scene;}

    void lower();
    void raise();
    void bringToFront();
    void sendToBack();
    void readAttributes(QDomElement);
    bool isSceneRect() {return m_isSceneRect;}

protected:
    void writeAttributes(QDomElement);

    QMenu *m_contextMenu;
    AnimationScene *m_scene;

private:
    bool m_deleted;
    bool m_isSceneRect;
    int m_playheadPosition;
    ItemHandle*  m_handles[8];
    bool m_hasHandles;
    QString m_id;
    QRectF m_rect;
    QPen m_pen;
    QBrush m_brush;
    QAction *delAct;
    QAction *bringToFrontAct;
    QAction *sendToBackAct;
    QAction *lowerAct;
    QAction *raiseAct;
    QHash<QString, KeyFrame*> *m_keyframes;
    qreal m_oldx, m_oldy, m_oldwidth, m_oldheight;
    int m_opacity;

private slots:
    void deleteItem();
    void bringToFrontAction();
    void sendToBackAction();
    void lowerAction();
    void raiseAction();

signals:
    void addPropertyAnimation(AnimationItem *item, const QString propertyName, qreal value, int min, int max);
    void idChanged(AnimationItem *item, QString value);
    void sizeChanged(qreal width, qreal height);
    void positionChanged(qreal x, qreal y);
    void itemRemoved(AnimationItem *item);
    void brushChanged(QColor);
    void penChanged(QColor);
    void opacityChanged(int opacity);

protected:
    void setHandlePositions();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
    qreal m_xscale;
    qreal m_yscale;

};

#endif // RESIZEABLEITEM_H
