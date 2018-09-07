#ifndef KEYFRAMEHANDLE_H
#define KEYFRAMEHANDLE_H

#include <QWidget>

#include "widgets_global.h"

class QMenu;
class TransitionLine;
class KeyFrame;
class AnimationItem;
class WIDGETSSHARED_EXPORT KeyframeHandle : public QWidget
{
    Q_OBJECT
public:
    KeyframeHandle(TransitionLine *parent, KeyFrame *key);

    void paintEvent(QPaintEvent *ev) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;

    KeyFrame *key() {return m_key;}

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

private slots:
    void onCustomContextMenu(const QPoint &point);
    void addTransition();
    void deleteKeyframe();

signals:
    void transitionAdded(KeyFrame *key);
    void keyframeDeleted(KeyframeHandle *handle);
    void keyframeMoved(KeyframeHandle *handle, int dist);

private:
    QImage m_imageRaute;
    QImage m_imageRauteHohl;
    QMenu *m_contextMenu;
    QAction *m_transitionAct;
    QAction *m_delKeyframeAct;
    KeyFrame *m_key;
    bool m_pressed;
};

#endif // KEYFRAMEHANDLE_H
