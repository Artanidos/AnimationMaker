#include "keyframehandle.h"
#include "keyframe.h"
#include "transitionline.h"
#include <QPainter>
#include <QMenu>

KeyframeHandle::KeyframeHandle(TransitionLine *parent, KeyFrame *key)
{
    m_key = key;
    m_pressed = false;

    setMouseTracking(true);
    setParent(parent);
    setCursor(Qt::SizeHorCursor);
    setMaximumHeight(11);
    setMaximumWidth(11);
    setVisible(true);
    setFocusPolicy(Qt::ClickFocus);

    m_imageRaute = QImage(":/images/raute-weiss.png");
    m_imageRauteHohl = QImage(":/images/raute-hohl.png");

    m_contextMenu = new QMenu();
    m_transitionAct = new QAction("Create transition");
    m_delKeyframeAct = new QAction("Delete keyframe");

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    connect(m_transitionAct, SIGNAL(triggered(bool)), this, SLOT(addTransition()));
    connect(m_delKeyframeAct, SIGNAL(triggered(bool)), this, SLOT(deleteKeyframe()));

    setContextMenuPolicy(Qt::CustomContextMenu);
}

void KeyframeHandle::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(m_key->prev() == nullptr || m_key->prev()->value() == m_key->value())
        painter.drawImage(0, 0, m_imageRaute);
    else
        painter.drawImage(0, 0, m_imageRauteHohl);

    if(hasFocus())
    {
        painter.setPen(QColor(255, 127, 42));
        painter.drawRect(0,0, width() - 1, height() - 1);
    }
}

void KeyframeHandle::onCustomContextMenu(const QPoint &point)
{
    m_contextMenu->clear();
    m_contextMenu->addAction(m_delKeyframeAct);
    if(m_key->prev() && m_key->prev()->easing() < 0)
        m_contextMenu->addAction(m_transitionAct);
    m_contextMenu->exec(this->mapToGlobal(point));
}

void KeyframeHandle::addTransition()
{
    emit transitionAdded(m_key->prev());
}

void KeyframeHandle::deleteKeyframe()
{
    emit keyframeDeleted(this);
}

void KeyframeHandle::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        m_pressed = true;
    }
}

void KeyframeHandle::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_pressed)
    {
        int p = x() + ev->x();
        int newVal = qRound((qreal)p * 5 / 100) * 100;
        move(newVal / 5 - 6, y());
    }
}

void KeyframeHandle::mouseReleaseEvent(QMouseEvent *ev)
{
    if(m_pressed)
    {
        m_pressed = false;
        int p = x() + ev->x();
        int newVal = qRound((qreal)p * 5 / 100) * 100;
        emit keyframeMoved(this, newVal);
    }
}

void KeyframeHandle::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
        case Qt::Key_Left:
            emit keyframeMoved(this, m_key->time() - 100);
            break;
        case Qt::Key_Right:
            emit keyframeMoved(this, m_key->time() + 100);
            break;
    }
}
