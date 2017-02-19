#include "playhead.h"
#include <QStyle>
#include <QPainter>
#include <QTest>

PlayHead::PlayHead(Qt::Orientation orientation)
    : QSlider(orientation)
{

}

void PlayHead::paintEvent(QPaintEvent *)
{
    QImage img(":/images/playhead.png");
    QColor gray = QColor(64, 66, 68);
    int width = size().width();
    int height = size().height();
    int position = QStyle::sliderPositionFromValue(minimum(), maximum(),value(), width);

    QPainter painter(this);

    painter.setPen(QColor(41, 41, 41));
    painter.fillRect(0, 0, width, height, gray);
    painter.drawRect(0, 0, width - 1, height - 1);

    painter.drawImage(position - 4, 1, img);
}
