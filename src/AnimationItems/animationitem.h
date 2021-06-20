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

#ifndef ANIMATIONITEM_H
#define ANIMATIONITEM_H

#include <QAction>
#include <QBrush>
#include <QColor>
#include <QDomDocument>
#include <QDomElement>
#include <QEvent>
#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QHash>
#include <QList>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QString>
#include <QStringList>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

#include "keyframe.h"
#include "itemhandle.h"

// Defined user type for custome AnimationItems
#define RECTANGLE_TYPE 1
#define ELLIPSE_TYPE 2
#define TEXT_TYPE 3
#define BITMAP_TYPE 4
#define SVG_TYPE 5


class AnimationScene;
class PropertyEditorInterface;

class  AnimationItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(qreal left READ left WRITE setX)
    Q_PROPERTY(qreal top READ top WRITE setY)
    Q_PROPERTY(qreal width READ top WRITE setWidth)
    Q_PROPERTY(qreal height READ top WRITE setHeight)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QColor brushColor READ brushColor WRITE setBrushColor)
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)
    Q_PROPERTY(qreal scalex READ scaleX WRITE setScaleX)
    Q_PROPERTY(qreal scaley READ scaleY WRITE setScaleY)
    Q_PROPERTY(qreal shearx READ shearX WRITE setShearX)
    Q_PROPERTY(qreal sheary READ shearY WRITE setShearY)
    Q_PROPERTY(int rotationx READ rotationX WRITE setRotationX)
    Q_PROPERTY(int rotationy READ rotationY WRITE setRotationY)
    Q_PROPERTY(int rotationz READ rotationZ WRITE setRotationZ)
    Q_INTERFACES(QGraphicsItem)

public:
    AnimationItem(AnimationScene *scene, bool isSceneRect = false);
    virtual ~AnimationItem() override;

    virtual QString typeName() = 0;
    virtual QDomElement getXml(QDomDocument) = 0;
    virtual QString getHtml(QString id, QString assetsPath) = 0;
    virtual bool hasBrushAndPen() = 0;
    virtual QList<PropertyEditorInterface*> *getPropertyEditors() {return nullptr;}
    virtual QStringList getPropertyList() {QStringList list; return list;}

    void setPlayheadPosition(int pos) {m_playheadPosition = pos; update();}
    int playheadPosition() {return m_playheadPosition;}
    void setScene(AnimationScene *scene);
    void drawHighlightSelected(QPainter *painter, const QStyleOptionGraphicsItem *option);
    void paint( QPainter *paint, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;
    bool sceneEventFilter(QGraphicsItem * watched, QEvent * event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual void scaleObjects();
    virtual void setScale(qreal x, qreal y);
    qreal xscale();
    qreal yscale();

    double scaleX() {return m_scaleX;}
    void setScaleX(double value);

    double scaleY() {return m_scaleY;}
    void setScaleY(double value);

    double shearX() {return m_shearX;}
    void setShearX(double value);

    double shearY() {return m_shearY;}
    void setShearY(double value);

    int rotationX() {return m_rotationX;}
    void setRotationX(int value);

    int rotationY() {return m_rotationY;}
    void setRotationY(int value);

    int rotationZ() {return m_rotationZ;}
    void setRotationZ(int value);

    int originX() {return m_originX;}
    void setOriginX(int value);

    int originY() {return m_originY;}
    void setOriginY(int value);

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

    void adjustKeyframes(QString propertyName, QVariant value, int time, bool autokeyframes, bool autotransition, KeyFrame **key, bool undo);

    void posChanged(qreal x, qreal y);

    inline AnimationScene *scene() {return m_scene;}

    void lower();
    void raise();
    void bringToFront();
    void sendToBack();
    void readAttributes(QDomElement);
    bool isSceneRect() {return m_isSceneRect;}

private:
    void doTransform();

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
    QList<QAction *> contextMenuActions;
    QHash<QString, KeyFrame*> *m_keyframes;
    qreal m_oldx, m_oldy, m_oldwidth, m_oldheight;
    int m_opacity;
    double m_scaleX;
    double m_scaleY;
    double m_shearX;
    double m_shearY;
    int m_rotationX;
    int m_rotationY;
    int m_rotationZ;
    int m_originX;
    int m_originY;

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
    void keyframeAdded(QString propertyName, KeyFrame *key);
    void keyframeRemoved(QString propertyName, KeyFrame *key);
    void transitionRemoved(QString propertyName, KeyFrame *key);
    void scaleXChanged(AnimationItem *item, double value);
    void scaleYChanged(AnimationItem *item, double value);
    void shearXChanged(AnimationItem *item, double value);
    void shearYChanged(AnimationItem *item, double value);
    void rotationXChanged(AnimationItem *item, int value);
    void rotationYChanged(AnimationItem *item, int value);
    void rotationZChanged(AnimationItem *item, int value);
    void originXChanged(AnimationItem *item, int value);
    void originYChanged(AnimationItem *item, int value);

protected:
    void setHandlePositions();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) Q_DECL_OVERRIDE;
    qreal m_xscale;
    qreal m_yscale;

};

#endif // ANIMATIONITEM_H
