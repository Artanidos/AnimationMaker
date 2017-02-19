#ifndef PLAYHEAD_H
#define PLAYHEAD_H

#include <QSlider>

class PlayHead : public QSlider
{
    Q_OBJECT
public:
    PlayHead(Qt::Orientation orientation);

    void paintEvent(QPaintEvent *ev);
//    inline void setMinimum(qreal val) {m_min = val; QSlider::setMinimum((int)val);}
//    inline void setMaximum(qreal val) {m_max = val; QSlider::setMaximum((int)val);}
//    inline void setValue(qreal val) {m_value = val; QSlider::setValue((int)val);}

//private:
//    qreal m_min;
//    qreal m_max;
//    qreal m_value;
};

#endif // PLAYHEAD_H
