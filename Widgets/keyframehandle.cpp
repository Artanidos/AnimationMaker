#include "keyframehandle.h"
#include "keyframe.h"
#include "transitionline.h"
#include <QPainter>
#include <QMenu>

KeyframeHandle::KeyframeHandle(TransitionLine *parent, KeyFrame *key)
{
    m_key = key;

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

}

void KeyframeHandle::mouseMoveEvent(QMouseEvent *ev)
{

}

void KeyframeHandle::mouseReleaseEvent(QMouseEvent *)
{

}

void KeyframeHandle::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
        case Qt::Key_Left:
            emit keyframeMoved(this, -1);
            break;
        case Qt::Key_Right:
            emit keyframeMoved(this, 1);
            break;
    }
}
