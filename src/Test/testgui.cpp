#include <QtWidgets>
#include <QtTest/QtTest>
#include "timeline.h"
#include "keyframehandle.h"
#include "rectangle.h"
#include "transition.h"
#include "transitionline.h"
#include "transitionhandleleft.h"
#include "transitionhandleright.h"

namespace QTest
{
    inline void myMouseMove(QWidget *widget, QPoint pos = QPoint(), int delay=-1)
    {
        QMouseEvent *ev = new QMouseEvent(QEvent::MouseMove, pos, Qt::LeftButton, Qt::LeftButton, Qt::KeyboardModifiers());
        QSpontaneKeyEvent::setSpontaneous(ev);
        qApp->notify(widget, ev);
    }
}

class TestGui: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testKeyframeHandle();
    void testKeyframeHandle_data();
    void testKeyframeHandleZero();
    void testKeyframeHandleZero_data();
    void testTransition();
    void testTransition_data();
    void testTransitionZero();
    void testTransitionZero_data();
    void testTransitionHandleLeft();
    void testTransitionHandleLeft_data();
    void testTransitionHandleRight();
    void testTransitionHandleRight_data();
    void testTransitionHandleLeftMouse();
    void testTransitionHandleRightMouse();
    void testTransitionMouse();
    void testDoubleTransition();

private:
    Timeline *m_timeline;
    AnimationScene *m_scene;
};

void TestGui::initTestCase()
{
    QUndoStack *undostack = new QUndoStack();
    m_scene = new AnimationScene();
    m_scene->registerUndoStack(undostack);
    m_timeline = new Timeline(m_scene);
    m_timeline->scrollValueChanged(0);
}

void TestGui::testKeyframeHandle_data()
{
    QTest::addColumn<QTestEventList>("events");
    QTest::addColumn<int>("expected");
    QTest::addColumn<int>("scrollpos");

    QTestEventList list1;
    list1.addMouseClick(Qt::LeftButton);
    list1.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right") << list1 << 400 << 0;

    QTestEventList list2;
    list2.addMouseClick(Qt::LeftButton);
    list2.addKeyClick(Qt::Key_Right);
    list2.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right 2 times") << list2 << 500 << 0;

    QTestEventList list3;
    list3.addMouseClick(Qt::LeftButton);
    list3.addKeyClick(Qt::Key_Right);
    list3.addKeyClick(Qt::Key_Right);
    list3.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right against other keyframe") << list3 << 500 << 0;

    QTestEventList list4;
    list4.addMouseClick(Qt::LeftButton);
    list4.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left") << list4 << 200 << 0;

    QTestEventList list5;
    list5.addMouseClick(Qt::LeftButton);
    list5.addKeyClick(Qt::Key_Left);
    list5.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left 2 times") << list5 << 100 << 0;

    QTestEventList list6;
    list6.addMouseClick(Qt::LeftButton);
    list6.addKeyClick(Qt::Key_Left);
    list6.addKeyClick(Qt::Key_Left);
    list6.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left against other keyframe") << list6 << 100 << 0;

    QTestEventList list7;
    list7.addMouseClick(Qt::LeftButton);
    list7.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list7.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20, 0));
    QTest::newRow("move right with mouse") << list7 << 400 << 0;

    QTestEventList list8;
    list8.addMouseClick(Qt::LeftButton);
    list8.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list8.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(40, 0));
    QTest::newRow("move right with mouse") << list8 << 500 << 0;

    QTestEventList list9;
    list9.addMouseClick(Qt::LeftButton);
    list9.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list9.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(60, 0));
    QTest::newRow("move right with mouse against other keyframe") << list9 << 500 << 0;

    QTestEventList list10;
    list10.addMouseClick(Qt::LeftButton);
    list10.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list10.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-20, 0));
    QTest::newRow("move left with mouse") << list10 << 200 << 0;

    QTestEventList list11;
    list11.addMouseClick(Qt::LeftButton);
    list11.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list11.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-40, 0));
    QTest::newRow("move left with mouse") << list11 << 100 << 0;

    QTestEventList list12;
    list12.addMouseClick(Qt::LeftButton);
    list12.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list12.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-60, 0));
    QTest::newRow("move left with mouse against other keyframe") << list12 << 100 << 0;

    QTestEventList list13;
    list13.addMouseClick(Qt::LeftButton);
    list13.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list13.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20, 0));
    QTest::newRow("move right with mouse") << list13 << 400 << 20;

    QTestEventList list14;
    list14.addMouseClick(Qt::LeftButton);
    list14.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list14.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(40, 0));
    QTest::newRow("move right with mouse") << list14 << 500 << 20;

    QTestEventList list15;
    list15.addMouseClick(Qt::LeftButton);
    list15.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list15.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(60, 0));
    QTest::newRow("move right with mouse against other keyframe") << list15 << 500 << 20;

    QTestEventList list16;
    list16.addMouseClick(Qt::LeftButton);
    list16.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list16.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-20, 0));
    QTest::newRow("move left with mouse") << list16 << 200 << 20;

    QTestEventList list17;
    list17.addMouseClick(Qt::LeftButton);
    list17.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list17.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-40, 0));
    QTest::newRow("move left with mouse") << list17 << 100 << 20;

    QTestEventList list18;
    list18.addMouseClick(Qt::LeftButton);
    list18.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list18.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-60, 0));
    QTest::newRow("move left with mouse against other keyframe") << list18 << 100 << 20;
}

void TestGui::testKeyframeHandle()
{
    QFETCH(QTestEventList, events);
    QFETCH(int, expected);
    QFETCH(int, scrollpos);

    AnimationItem *item = new Rectangle(m_scene);
    item->setWidth(50);
    item->setHeight(50);
    KeyFrame *key = new KeyFrame();
    key->setTime(300);
    KeyFrame *keyl = new KeyFrame();
    keyl->setTime(100);
    KeyFrame *keyr = new KeyFrame();
    keyr->setTime(500);
    m_timeline->scrollValueChanged(scrollpos);
    m_timeline->addKeyFrame(item, "left", keyl);
    m_timeline->addKeyFrame(item, "left", key);
    m_timeline->addKeyFrame(item, "left", keyr);

    TransitionLine *tl = m_timeline->getTransitionLine(item, "left");
    KeyframeHandle *kh = tl->getKeyframeHandle(key);
    events.simulate(kh);

    QCOMPARE(key->time(), expected);
}

void TestGui::testKeyframeHandleZero_data()
{
    QTest::addColumn<QTestEventList>("events");
    QTest::addColumn<int>("expected");

    QTestEventList list4;
    list4.addMouseClick(Qt::LeftButton);
    list4.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left") << list4 << 0;

    QTestEventList list5;
    list5.addMouseClick(Qt::LeftButton);
    list5.addKeyClick(Qt::Key_Left);
    list5.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left 2 times against zero") << list5 << 0;

    QTestEventList list10;
    list10.addMouseClick(Qt::LeftButton);
    list10.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list10.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-20, 0));
    QTest::newRow("move left with mouse") << list10 << 0;

    QTestEventList list11;
    list11.addMouseClick(Qt::LeftButton);
    list11.addMousePress(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    list11.addMouseRelease(Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-40, 0));
    QTest::newRow("move left with mouse against zero") << list11 << 0;
}

void TestGui::testKeyframeHandleZero()
{
    QFETCH(QTestEventList, events);
    QFETCH(int, expected);

    AnimationItem *item = new Rectangle(m_scene);
    item->setWidth(50);
    item->setHeight(50);
    KeyFrame *key = new KeyFrame();
    key->setTime(100);
    m_timeline->scrollValueChanged(0);
    m_timeline->addKeyFrame(item, "left", key);
    TransitionLine *tl = m_timeline->getTransitionLine(item, "left");
    KeyframeHandle *kh = tl->getKeyframeHandle(key);
    events.simulate(kh);

    QCOMPARE(key->time(), expected);
}

void TestGui::testTransition_data()
{
    QTest::addColumn<QTestEventList>("events");
    QTest::addColumn<int>("expected");

    QTestEventList list1;
    list1.addMouseClick(Qt::LeftButton);
    list1.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right") << list1 << 400;

    QTestEventList list2;
    list2.addMouseClick(Qt::LeftButton);
    list2.addKeyClick(Qt::Key_Right);
    list2.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right 2 times") << list2 << 500;

    QTestEventList list3;
    list3.addMouseClick(Qt::LeftButton);
    list3.addKeyClick(Qt::Key_Right);
    list3.addKeyClick(Qt::Key_Right);
    list3.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right 3 times against keyframe") << list3 << 500;

    QTestEventList list4;
    list4.addMouseClick(Qt::LeftButton);
    list4.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left") << list4 << 200;

    QTestEventList list5;
    list5.addMouseClick(Qt::LeftButton);
    list5.addKeyClick(Qt::Key_Left);
    list5.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left 2 times") << list5 << 100;

    QTestEventList list6;
    list6.addMouseClick(Qt::LeftButton);
    list6.addKeyClick(Qt::Key_Left);
    list6.addKeyClick(Qt::Key_Left);
    list6.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left 3 times against keyframe") << list6 <<100;
}

void TestGui::testTransition()
{
    QFETCH(QTestEventList, events);
    QFETCH(int, expected);

    AnimationItem *item = new Rectangle(m_scene);
    item->setWidth(50);
    item->setHeight(50);
    KeyFrame *keya = new KeyFrame();
    keya->setTime(300);
    keya->setEasing(0);
    KeyFrame *keyb = new KeyFrame();
    keyb->setTime(500);
    KeyFrame *keyl = new KeyFrame();
    keyl->setTime(100);
    KeyFrame *keyr = new KeyFrame();
    keyr->setTime(700);
    m_timeline->scrollValueChanged(0);
    m_timeline->addKeyFrame(item, "left", keyl);
    m_timeline->addKeyFrame(item, "left", keya);
    m_timeline->addKeyFrame(item, "left", keyb);
    m_timeline->addKeyFrame(item, "left", keyr);

    TransitionLine *tl = m_timeline->getTransitionLine(item, "left");
    Transition *tr = tl->getTransition(keya);
    events.simulate(tr);

    QCOMPARE(keya->time(), expected);
}

void TestGui::testTransitionZero_data()
{
    QTest::addColumn<QTestEventList>("events");
    QTest::addColumn<int>("expected");

    QTestEventList list4;
    list4.addMouseClick(Qt::LeftButton);
    list4.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left") << list4 << 0;

    QTestEventList list5;
    list5.addMouseClick(Qt::LeftButton);
    list5.addKeyClick(Qt::Key_Left);
    list5.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left 2 times against zero") << list5 << 0;
}

void TestGui::testTransitionZero()
{
    QFETCH(QTestEventList, events);
    QFETCH(int, expected);

    AnimationItem *item = new Rectangle(m_scene);
    item->setWidth(50);
    item->setHeight(50);
    KeyFrame *keya = new KeyFrame();
    keya->setTime(100);
    keya->setEasing(0);
    KeyFrame *keyb = new KeyFrame();
    keyb->setTime(200);
    m_timeline->scrollValueChanged(0);
    m_timeline->addKeyFrame(item, "left", keya);
    m_timeline->addKeyFrame(item, "left", keyb);
    TransitionLine *tl = m_timeline->getTransitionLine(item, "left");
    Transition *tr = tl->getTransition(keya);
    events.simulate(tr);

    QCOMPARE(keya->time(), expected);
}

void TestGui::testTransitionHandleLeft_data()
{
    QTest::addColumn<QTestEventList>("events");
    QTest::addColumn<int>("expected");

    QTestEventList list1;
    list1.addMouseClick(Qt::LeftButton);
    list1.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right") << list1 << 400;

    QTestEventList list2;
    list2.addMouseClick(Qt::LeftButton);
    list2.addKeyClick(Qt::Key_Right);
    list2.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right 2 times") << list2 << 500;

    QTestEventList list3;
    list3.addMouseClick(Qt::LeftButton);
    list3.addKeyClick(Qt::Key_Right);
    list3.addKeyClick(Qt::Key_Right);
    list3.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right 3 times") << list3 << 500;

    QTestEventList list4;
    list4.addMouseClick(Qt::LeftButton);
    list4.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left") << list4 << 200;

    QTestEventList list5;
    list5.addMouseClick(Qt::LeftButton);
    list5.addKeyClick(Qt::Key_Left);
    list5.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left 2 times") << list5 << 100;

    QTestEventList list6;
    list6.addMouseClick(Qt::LeftButton);
    list6.addKeyClick(Qt::Key_Left);
    list6.addKeyClick(Qt::Key_Left);
    list6.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left 3 times against keyframe") << list6 <<100;
}

void TestGui::testTransitionHandleLeft()
{
    QFETCH(QTestEventList, events);
    QFETCH(int, expected);

    AnimationItem *item = new Rectangle(m_scene);
    item->setWidth(50);
    item->setHeight(50);
    KeyFrame *keya = new KeyFrame();
    keya->setTime(300);
    keya->setEasing(0);
    KeyFrame *keyb = new KeyFrame();
    keyb->setTime(600);
    KeyFrame *keyl = new KeyFrame();
    keyl->setTime(100);
    m_timeline->scrollValueChanged(0);
    m_timeline->addKeyFrame(item, "left", keyl);
    m_timeline->addKeyFrame(item, "left", keya);
    m_timeline->addKeyFrame(item, "left", keyb);

    TransitionLine *tl = m_timeline->getTransitionLine(item, "left");
    Transition *tr = tl->getTransition(keya);
    TransitionHandleLeft *thl = static_cast<TransitionHandleLeft *>(tr->getLeftHandle());
    events.simulate(thl);

    QCOMPARE(keya->time(), expected);
}

void TestGui::testTransitionHandleRight_data()
{
    QTest::addColumn<QTestEventList>("events");
    QTest::addColumn<int>("expected");

    QTestEventList list1;
    list1.addMouseClick(Qt::LeftButton);
    list1.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right") << list1 << 600;

    QTestEventList list2;
    list2.addMouseClick(Qt::LeftButton);
    list2.addKeyClick(Qt::Key_Right);
    list2.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right 2 times") << list2 << 700;

    QTestEventList list3;
    list3.addMouseClick(Qt::LeftButton);
    list3.addKeyClick(Qt::Key_Right);
    list3.addKeyClick(Qt::Key_Right);
    list3.addKeyClick(Qt::Key_Right);
    QTest::newRow("move right 3 times against keyframe") << list3 << 700;

    QTestEventList list4;
    list4.addMouseClick(Qt::LeftButton);
    list4.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left") << list4 << 400;

    QTestEventList list5;
    list5.addMouseClick(Qt::LeftButton);
    list5.addKeyClick(Qt::Key_Left);
    list5.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left 2 times") << list5 << 300;

    QTestEventList list6;
    list6.addMouseClick(Qt::LeftButton);
    list6.addKeyClick(Qt::Key_Left);
    list6.addKeyClick(Qt::Key_Left);
    list6.addKeyClick(Qt::Key_Left);
    QTest::newRow("move left 3 times") << list6 << 300;
}

void TestGui::testTransitionHandleRight()
{
    QFETCH(QTestEventList, events);
    QFETCH(int, expected);

    AnimationItem *item = new Rectangle(m_scene);
    item->setWidth(50);
    item->setHeight(50);
    KeyFrame *keya = new KeyFrame();
    keya->setTime(200);
    keya->setEasing(0);
    KeyFrame *keyb = new KeyFrame();
    keyb->setTime(500);
    KeyFrame *keyr = new KeyFrame();
    keyr->setTime(700);
    m_timeline->scrollValueChanged(0);
    m_timeline->addKeyFrame(item, "left", keya);
    m_timeline->addKeyFrame(item, "left", keyb);
    m_timeline->addKeyFrame(item, "left", keyr);

    TransitionLine *tl = m_timeline->getTransitionLine(item, "left");
    Transition *tr = tl->getTransition(keya);
    TransitionHandleRight *thr = static_cast<TransitionHandleRight *>(tr->getRightHandle());
    events.simulate(thr);

    QCOMPARE(keyb->time(), expected);
}

void TestGui::testTransitionHandleLeftMouse()
{
    AnimationItem *item = new Rectangle(m_scene);
    item->setWidth(50);
    item->setHeight(50);
    KeyFrame *keya = new KeyFrame();
    keya->setTime(200);
    keya->setEasing(0);
    KeyFrame *keyb = new KeyFrame();
    keyb->setTime(500);
    KeyFrame *keyl = new KeyFrame();
    keyl->setTime(100);
    KeyFrame *keyr = new KeyFrame();
    keyr->setTime(700);
    m_timeline->scrollValueChanged(0);
    m_timeline->addKeyFrame(item, "left", keya);
    m_timeline->addKeyFrame(item, "left", keyb);
    m_timeline->addKeyFrame(item, "left", keyl);
    m_timeline->addKeyFrame(item, "left", keyr);

    TransitionLine *tl = m_timeline->getTransitionLine(item, "left");
    Transition *tr = tl->getTransition(keya);
    TransitionHandleLeft *thl = static_cast<TransitionHandleLeft *>(tr->getLeftHandle());

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(20, 0));
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20, 0));
    QCOMPARE(keya->time(), 300);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(20, 0));
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20, 0));
    QCOMPARE(keya->time(), 400);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(20, 0));
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20, 0));
    QCOMPARE(keya->time(), 400);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(-20, 0));
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-20, 0));
    QCOMPARE(keya->time(), 300);

    m_scene->undoStack()->undo();
    QCOMPARE(keya->time(), 400);

    m_scene->undoStack()->redo();
    QCOMPARE(keya->time(), 300);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(-40, 0));
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-40, 0));
    QCOMPARE(keya->time(), 100);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(-20, 0));
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-20, 0));
    QCOMPARE(keya->time(), 100);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(40, 0));
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(40, 0));
    QCOMPARE(keya->time(), 300);

    m_scene->undoStack()->undo();
    QCOMPARE(keya->time(), 100);

    m_scene->undoStack()->redo();
    QCOMPARE(keya->time(), 300);

    // test moving against zero
    m_timeline->deleteKeyFrame(item, "left", keyl);
    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(-80, 0));
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-80, 0));
    QCOMPARE(keya->time(), 0);

    // change scroll pos
    m_timeline->scrollValueChanged(20);
    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(20, 0));
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20, 0));
    QCOMPARE(keya->time(), 100);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(-20, 0));
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-20, 0));
    QCOMPARE(keya->time(), 0);
}

void TestGui::testTransitionHandleRightMouse()
{
    AnimationItem *item = new Rectangle(m_scene);
    item->setWidth(50);
    item->setHeight(50);
    KeyFrame *keya = new KeyFrame();
    keya->setTime(200);
    keya->setEasing(0);
    KeyFrame *keyb = new KeyFrame();
    keyb->setTime(500);
    KeyFrame *keyl = new KeyFrame();
    keyl->setTime(100);
    KeyFrame *keyr = new KeyFrame();
    keyr->setTime(700);
    m_timeline->scrollValueChanged(0);
    m_timeline->addKeyFrame(item, "left", keya);
    m_timeline->addKeyFrame(item, "left", keyb);
    m_timeline->addKeyFrame(item, "left", keyl);
    m_timeline->addKeyFrame(item, "left", keyr);

    TransitionLine *tl = m_timeline->getTransitionLine(item, "left");
    Transition *tr = tl->getTransition(keya);
    TransitionHandleRight *thr = static_cast<TransitionHandleRight *>(tr->getRightHandle());

    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(20, 0));
    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20, 0));
    QCOMPARE(keyb->time(), 600);

    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(20, 0));
    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20, 0));
    QCOMPARE(keyb->time(), 700);

    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(20, 0));
    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20, 0));
    QCOMPARE(keyb->time(), 700);

    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(-20, 0));
    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-20, 0));
    QCOMPARE(keyb->time(), 600);

    m_scene->undoStack()->undo();
    QCOMPARE(keyb->time(), 700);

    m_scene->undoStack()->redo();
    QCOMPARE(keyb->time(), 600);

    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(-40, 0));
    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-40, 0));
    QCOMPARE(keyb->time(), 400);

    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(-40, 0));
    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-40, 0));
    QCOMPARE(keyb->time(), 300);

    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(40, 0));
    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(40, 0));
    QCOMPARE(keyb->time(), 500);

    m_scene->undoStack()->undo();
    QCOMPARE(keyb->time(), 300);

    m_scene->undoStack()->redo();
    QCOMPARE(keyb->time(), 500);

    // change scroll pos
    m_timeline->scrollValueChanged(20);
    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(20, 0));
    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20, 0));
    QCOMPARE(keyb->time(), 600);

    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(-20, 0));
    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-20, 0));
    QCOMPARE(keyb->time(), 500);
}

void TestGui::testTransitionMouse()
{
    AnimationItem *item = new Rectangle(m_scene);
    item->setWidth(50);
    item->setHeight(50);
    KeyFrame *keya = new KeyFrame();
    keya->setTime(200);
    keya->setEasing(0);
    KeyFrame *keyb = new KeyFrame();
    keyb->setTime(500);
    KeyFrame *keyl = new KeyFrame();
    keyl->setTime(100);
    KeyFrame *keyr = new KeyFrame();
    keyr->setTime(700);
    m_timeline->scrollValueChanged(0);
    m_timeline->addKeyFrame(item, "left", keya);
    m_timeline->addKeyFrame(item, "left", keyb);
    m_timeline->addKeyFrame(item, "left", keyl);
    m_timeline->addKeyFrame(item, "left", keyr);

    TransitionLine *tl = m_timeline->getTransitionLine(item, "left");
    Transition *tr = tl->getTransition(keya);

    QTest::mousePress(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(tr, QPoint(60, 0));
    QTest::mouseRelease(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(60, 0));
    QCOMPARE(keya->time(), 400);
    QCOMPARE(keyb->time(), 700);

    m_scene->undoStack()->undo();
    QCOMPARE(keya->time(), 200);
    QCOMPARE(keyb->time(), 500);

    m_scene->undoStack()->redo();
    QCOMPARE(keya->time(), 400);
    QCOMPARE(keyb->time(), 700);

    QTest::mousePress(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(tr, QPoint(60, 0));
    QTest::mouseRelease(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(60, 0));
    QCOMPARE(keya->time(), 400);
    QCOMPARE(keyb->time(), 700);

    QTest::mousePress(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(tr, QPoint(-20, 0));
    QTest::mouseRelease(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-20, 0));
    QCOMPARE(keya->time(), 200);
    QCOMPARE(keyb->time(), 500);

    m_scene->undoStack()->undo();
    QCOMPARE(keya->time(), 400);
    QCOMPARE(keyb->time(), 700);

    m_scene->undoStack()->redo();
    QCOMPARE(keya->time(), 200);
    QCOMPARE(keyb->time(), 500);

    QTest::mousePress(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(tr, QPoint(-40, 0));
    QTest::mouseRelease(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-40, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 400);

    // test moving against zero
    m_timeline->deleteKeyFrame(item, "left", keyl);
    QTest::mousePress(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(tr, QPoint(-40, 0));
    QTest::mouseRelease(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-40, 0));
    QCOMPARE(keya->time(), 0);
    QCOMPARE(keyb->time(), 300);

    // change scroll pos
    m_timeline->scrollValueChanged(20);
    QTest::mousePress(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(tr, QPoint(40, 0));
    QTest::mouseRelease(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(40, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 400);

    m_timeline->scrollValueChanged(20);
    QTest::mousePress(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(tr, QPoint(-40, 0));
    QTest::mouseRelease(tr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-40, 0));
    QCOMPARE(keya->time(), 0);
    QCOMPARE(keyb->time(), 300);
}

void TestGui::testDoubleTransition()
{
    AnimationItem *item = new Rectangle(m_scene);
    item->setWidth(50);
    item->setHeight(50);
    KeyFrame *keya = new KeyFrame();
    keya->setTime(200);
    KeyFrame *keyb = new KeyFrame();
    keyb->setTime(500);
    KeyFrame *keyc = new KeyFrame();
    keyc->setTime(700);
    KeyFrame *keyl = new KeyFrame();
    keyl->setTime(100);
    KeyFrame *keyr = new KeyFrame();
    keyr->setTime(900);
    m_timeline->scrollValueChanged(0);
    m_timeline->addKeyFrame(item, "left", keyc);
    m_timeline->addKeyFrame(item, "left", keyb);
    m_timeline->addKeyFrame(item, "left", keya);
    m_timeline->addKeyFrame(item, "left", keyl);
    m_timeline->addKeyFrame(item, "left", keyr);
    keya->setEasing(0);
    m_timeline->addTransition(item, "left", keya);
    keyb->setEasing(0);
    m_timeline->addTransition(item, "left", keyb);

    TransitionLine *tl = m_timeline->getTransitionLine(item, "left");
    Transition *trl = tl->getTransition(keya);
    Transition *trr = tl->getTransition(keyb);
    TransitionHandleLeft *thl = static_cast<TransitionHandleLeft *>(trr->getLeftHandle());
    TransitionHandleRight *thr = static_cast<TransitionHandleRight *>(trl->getRightHandle());

    QTest::mousePress(trl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(trl, QPoint(40, 0));
    QTest::mouseRelease(trl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(40, 0));
    QCOMPARE(keya->time(), 300);
    QCOMPARE(keyb->time(), 600);
    QCOMPARE(keyc->time(), 700);

    m_scene->undoStack()->undo();
    QCOMPARE(keya->time(), 200);
    QCOMPARE(keyb->time(), 500);
    QCOMPARE(keyc->time(), 700);

    m_scene->undoStack()->redo();
    QCOMPARE(keya->time(), 300);
    QCOMPARE(keyb->time(), 600);
    QCOMPARE(keyc->time(), 700);

    QTest::mousePress(trl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(trl, QPoint(40, 0));
    QTest::mouseRelease(trl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(40, 0));
    QCOMPARE(keya->time(), 300);
    QCOMPARE(keyb->time(), 600);
    QCOMPARE(keyc->time(), 700);

    QTest::mousePress(trl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(trl, QPoint(-40, 0));
    QTest::mouseRelease(trl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-40, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 400);
    QCOMPARE(keyc->time(), 700);

    QTest::mousePress(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(trr, QPoint(40, 0));
    QTest::mouseRelease(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(40, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 500);
    QCOMPARE(keyc->time(), 800);

    QTest::mousePress(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(trr, QPoint(60, 0));
    QTest::mouseRelease(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(60, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 600);
    QCOMPARE(keyc->time(), 900);

    m_scene->undoStack()->undo();
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 500);
    QCOMPARE(keyc->time(), 800);

    m_scene->undoStack()->redo();
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 600);
    QCOMPARE(keyc->time(), 900);

    QTest::mousePress(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(trr, QPoint(0, 0));
    QTest::mouseRelease(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 500);
    QCOMPARE(keyc->time(), 800);

    QTest::mousePress(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::myMouseMove(trr, QPoint(-80, 0));
    QTest::mouseRelease(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-80, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 200);
    QCOMPARE(keyc->time(), 500);

    m_scene->undoStack()->undo();
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 500);
    QCOMPARE(keyc->time(), 800);

    m_scene->undoStack()->redo();
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 200);
    QCOMPARE(keyc->time(), 500);

    QTest::mousePress(trl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(trl, Qt::Key_Right);
    QCOMPARE(keya->time(), 200);
    QCOMPARE(keyb->time(), 300);
    QCOMPARE(keyc->time(), 500);

    m_scene->undoStack()->undo();
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 200);
    QCOMPARE(keyc->time(), 500);

    m_scene->undoStack()->redo();
    QCOMPARE(keya->time(), 200);
    QCOMPARE(keyb->time(), 300);
    QCOMPARE(keyc->time(), 500);

    QTest::mousePress(trl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(trl, Qt::Key_Right);
    QCOMPARE(keya->time(), 300);
    QCOMPARE(keyb->time(), 400);
    QCOMPARE(keyc->time(), 500);

    QTest::mousePress(trl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(trl, Qt::Key_Right);
    QCOMPARE(keya->time(), 300);
    QCOMPARE(keyb->time(), 400);
    QCOMPARE(keyc->time(), 500);

    QTest::mousePress(trl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(trl, Qt::Key_Left);
    QCOMPARE(keya->time(), 200);
    QCOMPARE(keyb->time(), 300);
    QCOMPARE(keyc->time(), 500);

    QTest::mousePress(trl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(trl, Qt::Key_Left);
    QTest::keyClick(trl, Qt::Key_Left);
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 200);
    QCOMPARE(keyc->time(), 500);

    QTest::mousePress(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(trr, Qt::Key_Right);
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 300);
    QCOMPARE(keyc->time(), 600);

    QTest::mousePress(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(trr, Qt::Key_Right);
    QTest::keyClick(trr, Qt::Key_Right);
    QTest::keyClick(trr, Qt::Key_Right);
    QTest::keyClick(trr, Qt::Key_Right);
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 600);
    QCOMPARE(keyc->time(), 900);

    m_scene->undoStack()->undo();
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 500);
    QCOMPARE(keyc->time(), 800);

    m_scene->undoStack()->redo();
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 600);
    QCOMPARE(keyc->time(), 900);

    QTest::mousePress(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(trr, Qt::Key_Left);
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 500);
    QCOMPARE(keyc->time(), 800);

    QTest::mousePress(trr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(trr, Qt::Key_Left);
    QTest::keyClick(trr, Qt::Key_Left);
    QTest::keyClick(trr, Qt::Key_Left);
    QTest::keyClick(trr, Qt::Key_Left);
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 200);
    QCOMPARE(keyc->time(), 500);

    m_scene->undoStack()->undo();
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 300);
    QCOMPARE(keyc->time(), 600);

    m_scene->undoStack()->redo();
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 200);
    QCOMPARE(keyc->time(), 500);

    // testing handles
    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(thr, Qt::Key_Right);
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 300);
    QCOMPARE(keyc->time(), 500);
    QCOMPARE(trl->width(), 40);
    QCOMPARE(trr->width(), 40);

    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(thr, Qt::Key_Right);
    QTest::keyClick(thr, Qt::Key_Right);
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 400);
    QCOMPARE(keyc->time(), 500);
    QCOMPARE(trl->width(), 60);
    QCOMPARE(trr->width(), 20);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(thl, Qt::Key_Right);
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 400);
    QCOMPARE(keyc->time(), 500);
    QCOMPARE(trl->width(), 60);
    QCOMPARE(trr->width(), 20);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(thl, Qt::Key_Left);
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 300);
    QCOMPARE(keyc->time(), 500);
    QCOMPARE(trl->width(), 40);
    QCOMPARE(trr->width(), 40);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20,0));
    QTest::keyClick(thl, Qt::Key_Left);
    QTest::keyClick(thl, Qt::Key_Left);
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 200);
    QCOMPARE(keyc->time(), 500);
    QCOMPARE(trl->width(), 20);
    QCOMPARE(trr->width(), 60);

    // testing mousemove
    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(20, 0));
    QCOMPARE(trl->width(), 40);
    QCOMPARE(trr->width(), 40);

    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(20, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 300);
    QCOMPARE(keyc->time(), 500);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(-20, 0));
    QCOMPARE(trl->width(), 20);
    QCOMPARE(trr->width(), 60);
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-20, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 200);
    QCOMPARE(keyc->time(), 500);
    QCOMPARE(trl->width(), 20);
    QCOMPARE(trr->width(), 60);

    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(60, 0));
    QCOMPARE(trl->width(), 60);
    QCOMPARE(trr->width(), 20);
    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(60, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 400);
    QCOMPARE(keyc->time(), 500);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(-60, 0));
    QCOMPARE(trl->width(), 20);
    QCOMPARE(trr->width(), 60);
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-60, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 200);
    QCOMPARE(keyc->time(), 500);
    QCOMPARE(trl->width(), 20);
    QCOMPARE(trr->width(), 60);

    QTest::mousePress(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thl, QPoint(60, 0));
    QCOMPARE(trl->width(), 60);
    QCOMPARE(trr->width(), 20);
    QTest::mouseRelease(thl, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(60, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 400);
    QCOMPARE(keyc->time(), 500);
    QCOMPARE(trl->width(), 60);
    QCOMPARE(trr->width(), 20);

    QTest::mousePress(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(0,0));
    QTest::myMouseMove(thr, QPoint(-60, 0));
    QCOMPARE(trl->width(), 20);
    QCOMPARE(trr->width(), 60);
    QTest::mouseRelease(thr, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(-60, 0));
    QCOMPARE(keya->time(), 100);
    QCOMPARE(keyb->time(), 200);
    QCOMPARE(keyc->time(), 500);
    QCOMPARE(trl->width(), 20);
    QCOMPARE(trr->width(), 60);
}

QTEST_MAIN(TestGui)
#include "testgui.moc"
