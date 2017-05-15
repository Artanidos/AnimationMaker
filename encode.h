#ifndef ENCODE_H
#define ENCODE_H

class QGraphicsView;
class AnimationScene;
class MainWindow;

int video_encode(const char *filename, QGraphicsView *view, int length, MainWindow *win, AnimationScene *scene);

#endif // ENCODE_H
