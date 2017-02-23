#ifndef PLAYHEAD_H
#define PLAYHEAD_H

#include <QSlider>

class PlayHead : public QSlider
{
    Q_OBJECT
public:
    PlayHead(Qt::Orientation orientation);

    void paintEvent(QPaintEvent *ev);

private:
    QImage m_image;
};

#endif // PLAYHEAD_H
