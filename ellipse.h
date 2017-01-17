#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QtQuick/QQuickPaintedItem>

class EllipseBorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(qreal width READ width WRITE setWidth)

public:
    EllipseBorder(QObject *parent = 0);

    QColor color() const
    {
        return m_color;
    }
    void setColor(const QColor &color)
    {
        m_color = color;
    }
    qreal width() const
    {
        return m_width;
    }
    void setWidth(qreal width)
    {
        m_width = width;
    }

private:
    qreal m_width;
    QColor m_color;
};

class Ellipse : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(EllipseBorder * border READ border)

public:
    Ellipse(QQuickPaintedItem *parent = 0);

    void paint(QPainter *painter);

    QColor color() const
    {
        return m_color;
    }
    void setColor(const QColor &color)
    {
        m_color = color;
    }
    EllipseBorder *border()
    {
        return &m_border;
    }

private:
    EllipseBorder m_border;
    QColor m_color;
};

#endif // ELLIPSE_H
