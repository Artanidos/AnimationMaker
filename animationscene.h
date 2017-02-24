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
    ResizeableItem *m_copy;
};

QDataStream &operator<<(QDataStream &, const AnimationScene *);
QDataStream &operator>>(QDataStream &, AnimationScene *);
QString getItemName(ResizeableItem *item);
QString getItemTypeName(ResizeableItem *item);
bool isAnimationMakerItem(QGraphicsItem *item);

#endif // ANIMATIONSCENE_H
