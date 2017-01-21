#ifndef ANIMATIONSCENE_H
#define ANIMATIONSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDataStream>

class AnimationScene : public QGraphicsScene
{
public:
    AnimationScene();

    enum ItemType { Item, Rectangle, Ellipse, Text };
    QDataStream& read(QDataStream &dataStream);
    QDataStream& write(QDataStream &dataStream) const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
};

QDataStream &operator<<(QDataStream &, const AnimationScene *);
QDataStream &operator>>(QDataStream &, AnimationScene *);

#endif // ANIMATIONSCENE_H
