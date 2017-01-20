#include "animationscene.h"

AnimationScene::AnimationScene()
{

}


void AnimationScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //if (mouseEvent->button() != Qt::LeftButton)
    //    return;
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void AnimationScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}
void AnimationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
