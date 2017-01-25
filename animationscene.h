#ifndef ANIMATIONSCENE_H
#define ANIMATIONSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDataStream>

class AnimationScene : public QGraphicsScene
{
    Q_OBJECT
public:
    AnimationScene();

    enum ItemType { TypeItem, TypeRectangle, TypeEllipse, TypeText };
    enum EditMode { ModeSelect, ModeRectangle, ModeEllipse };

    void setEditMode(EditMode mode);
    QDataStream& read(QDataStream &dataStream);
    QDataStream& write(QDataStream &dataStream) const;
    void deselectAll();

signals:
    void itemAdded(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

private:
    EditMode m_editMode;
};

QDataStream &operator<<(QDataStream &, const AnimationScene *);
QDataStream &operator>>(QDataStream &, AnimationScene *);

#endif // ANIMATIONSCENE_H
