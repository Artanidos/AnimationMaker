#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QSlider>

class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    ColorPicker();

    void paintEvent(QPaintEvent *ev);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    QSize sizeHint() const;

    qreal hue();
    void setHue(qreal value);

signals:
    void colorChanged(QColor color);

private:
    qreal m_hue;
    bool m_lpressed;
    QColor getColor(QMouseEvent *event);
};

#endif // COLORPICKER_H
