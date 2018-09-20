/****************************************************************************
** Copyright (C) 2016 Olaf Japp
**
** This file is part of AnimationMaker.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "timeline.h"
#include "animationitem.h"
#include "keyframe.h"
#include "commands.h"
#include "transition.h"
#include <QHeaderView>
#include <QLabel>
#include <QGridLayout>
#include <QMenu>
#include <QTest>
#include <QScrollBar>

Timeline::Timeline(AnimationScene *scene)
    : QWidget(nullptr)
{
    m_scene = scene;

    QHBoxLayout *hbox = new QHBoxLayout();
    QToolButton *revertButton = new QToolButton();
    QToolButton *forwardButton = new QToolButton();
    QToolButton *autokeyframeButton = new QToolButton();
    autokeyframeButton->setCheckable(true);
    autokeyframeButton->setChecked(false);
    autokeyframeButton->setIcon(QIcon(":/images/autokeyframe.png"));
    autokeyframeButton->setToolTip("Auto keyframes");
    connect(autokeyframeButton, SIGNAL(toggled(bool)), this, SLOT(autokeyframes(bool)));

    QToolButton *autotransitionButton = new QToolButton();
    autotransitionButton->setCheckable(true);
    autotransitionButton->setChecked(false);
    autotransitionButton->setIcon(QIcon(":/images/autotransition.png"));
    autotransitionButton->setToolTip("Auto transitions");
    connect(autotransitionButton, SIGNAL(toggled(bool)), this, SLOT(autotransitions(bool)));
    playButton = new QToolButton();
    pauseButton = new QToolButton();
    pauseButton->setVisible(false);
    m_time = new QLabel();
    m_time->setText("0:00.0");

    QAction *playAct = new QAction("Play");
    playAct->setIcon(QIcon(":/images/play.png"));
    playAct->setToolTip("Start the animation");
    connect(playAct, SIGNAL(triggered()), this, SLOT(playAnimation()));

    QAction *pauseAct = new QAction("Pause");
    pauseAct->setIcon(QIcon(":/images/pause.png"));
    pauseAct->setToolTip("Pause the animation");
    connect(pauseAct, SIGNAL(triggered()), this, SLOT(pauseAnimation()));

    QAction *reverseAct = new QAction("Reverse");
    reverseAct->setIcon(QIcon(":/images/reverse.png"));
    reverseAct->setToolTip("Reverse the animation");
    connect(reverseAct, SIGNAL(triggered()), this, SLOT(revertAnimation()));

    QAction *forwardAct = new QAction("Forward");
    forwardAct->setIcon(QIcon(":/images/forward.png"));
    forwardAct->setToolTip("Forward the animation");
    connect(forwardAct, SIGNAL(triggered()), this, SLOT(forwardAnimation()));

    revertButton->setDefaultAction(reverseAct);
    playButton->setDefaultAction(playAct);
    pauseButton->setDefaultAction(pauseAct);
    forwardButton->setDefaultAction(forwardAct);

    hbox->addWidget(revertButton);
    hbox->addWidget(playButton);
    hbox->addWidget(pauseButton);
    hbox->addWidget(forwardButton);
    hbox->addStretch();
    hbox->addWidget(autokeyframeButton);
    hbox->addWidget(autotransitionButton);
    hbox->addSpacing(5);
    hbox->addWidget(m_time);
    QGridLayout *layout = new QGridLayout;
    m_tree = new QTreeWidget();
    m_tree->header()->close();
    m_tree->setStyleSheet("QTreeWidget::item:has-children:!selected {background-color: #4c4e50;} QTreeWidget::item:!selected { border-bottom: 1px solid #292929;} QTreeView::branch:!selected {border-bottom: 1px solid #292929;} QTreeWidget::branch:has-children:!selected {background-color: #4c4e50;} QTreeWidget::branch:has-children:!has-siblings:closed, QTreeWidget::branch:closed:has-children:has-siblings {border-image: none; image: url(:/images/branch-closed.png);} QTreeWidget::branch:open:has-children:!has-siblings, QTreeWidget::branch:open:has-children:has-siblings {border-image: none;image: url(:/images/branch-open.png);}");
    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);

    m_transitionPanel = new TransitionPanel(m_scene->undoStack());
    m_transitionPanel->setTreeWidget(m_tree);
    m_transitionPanel->registerTimeline(this);
    m_playhead = new PlayHead();
    m_sb = new QScrollBar(Qt::Horizontal);
    m_sb->setMaximum(50);

    layout->addItem(hbox, 0, 0);
    layout->addWidget(m_playhead, 0, 1);
    layout->addWidget(m_tree, 1, 0);
    layout->addWidget(m_transitionPanel, 1, 1);
    layout->addWidget(m_sb, 2, 1);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,1);

    this->setLayout(layout);

    m_contextMenu = new QMenu();
    m_delAct = new QAction("Delete");

    connect(m_tree, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    connect(m_tree, SIGNAL(expanded(QModelIndex)), m_transitionPanel, SLOT(treeExpanded(QModelIndex)));
    connect(m_tree, SIGNAL(collapsed(QModelIndex)), m_transitionPanel, SLOT(treeCollapsed(QModelIndex)));
    connect(this, SIGNAL(lineAdded(AnimationItem*)), m_transitionPanel, SLOT(lineAdded(AnimationItem *)));
    connect(this, SIGNAL(propertyAdded(AnimationItem*,QString)), m_transitionPanel, SLOT(propertyAdded(AnimationItem *, QString)));
    connect(this, SIGNAL(propertyKeyAdded(AnimationItem*,QString,KeyFrame*)), m_transitionPanel, SLOT(propertyKeyAdded(AnimationItem*, QString, KeyFrame*)));
    connect(this, SIGNAL(propertyKeyRemoved(AnimationItem*,QString,KeyFrame*)), m_transitionPanel, SLOT(propertyKeyRemoved(AnimationItem*, QString, KeyFrame*)));
    connect(this, SIGNAL(keyframeDeleted(AnimationItem*,QString)), m_transitionPanel, SLOT(deleteKeyframe(AnimationItem*,QString)));

    connect(m_tree->verticalScrollBar(), SIGNAL(valueChanged(int)), m_transitionPanel, SLOT(treeScrollValueChanged(int)));

    connect(m_playhead, SIGNAL(valueChanged(int)), this, SLOT(playheadValueChanged(int)));
    connect(m_sb, SIGNAL(valueChanged(int)), m_transitionPanel, SLOT(scrollValueChanged(int)));
    connect(m_sb, SIGNAL(valueChanged(int)), m_playhead, SLOT(scrollValueChanged(int)));
    connect(m_sb, SIGNAL(valueChanged(int)), this, SLOT(scrollValueChanged(int)));

    connect(m_tree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(treeCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(m_playhead, SIGNAL(valueChanged(int)), scene, SLOT(setPlayheadPosition(int)));
    connect(m_scene, SIGNAL(keyframeAdded(AnimationItem*, QString, KeyFrame*)), this, SLOT(keyframeAdded(AnimationItem*, QString, KeyFrame*)));
}

void Timeline::scrollValueChanged(int value)
{

    int max = m_sb->maximum();
    if(value > max * 90 / 100)
        m_sb->setMaximum(max * 110 / 100);
}

void Timeline::reset()
{
    while(1)
    {
        QTreeWidgetItem *treeItem = m_tree->takeTopLevelItem(0);
        if(treeItem == nullptr)
            break;
        delete treeItem;
    }
    m_transitionPanel->reset();
    m_playhead->setValue(0);
    m_sb->setMaximum(50);
}

void Timeline::transitionSelected(KeyFrame *frame)
{
    emit transitionSelectionChanged(frame);
}

void Timeline::transitionDeselected()
{
    emit transitionSelectionChanged(nullptr);
}

void Timeline::onCustomContextMenu(const QPoint &point)
{
    QTreeWidgetItem *treeItem = m_tree->itemAt(point);
    if(treeItem)
    {
        m_contextMenu->clear();
        m_contextMenu->addAction(m_delAct);
        m_contextMenu->exec(m_tree->mapToGlobal(point));
    }
}

void Timeline::playAnimation()
{
    m_scene->clearSelection();

    int delay = 1000 / m_scene->fps();
    int last = lastKeyframe();
    int frames = last / delay;
    m_playing = true;
    playButton->setVisible(false);
    pauseButton->setVisible(true);

    for(int i=0; i < frames; i++)
    {
        m_playhead->setValue(i * delay);
        QTest::qSleep(delay / 2);
        QCoreApplication::processEvents(QEventLoop::AllEvents, delay / 2);
        if(!m_playing)
            break;
    }
    if(m_playing)
        m_playhead->setValue(lastKeyframe());
    pauseButton->setVisible(false);
    playButton->setVisible(true);
    m_playing = false;
}

void Timeline::pauseAnimation()
{
    m_playing = false;
}

void Timeline::revertAnimation()
{
    m_scene->clearSelection();
    m_playhead->setValue(0);
}

void Timeline::forwardAnimation()
{
    m_scene->clearSelection();

    int last = lastKeyframe();
    m_playhead->setValue(last);
}

QString timeString(int val, bool showMinutes)
{
    int minutes = val / 60000;
    int seconds = (val - minutes * 60000) / 1000;
    int milliseconds = val - minutes * 60000 - seconds * 1000;
    QString txt;
    if(minutes > 0 || showMinutes)
        txt.sprintf("%d:%02d.%d", minutes, seconds, milliseconds / 100);
    else
        txt.sprintf("%2d.%d", seconds, milliseconds / 100);
    return txt;
}

void Timeline::playheadValueChanged(int val)
{
    m_time->setText(timeString(val));
    m_scene->clearSelection();
    m_scene->setPlayheadPosition(val);
    m_transitionPanel->setPlayheadPosition(val);
}

void Timeline::treeCurrentItemChanged(QTreeWidgetItem *currentItem, QTreeWidgetItem*)
{
    if(currentItem)
    {
        QVariant level = currentItem->data(1,0);
        if(level == 1)
        {
            AnimationItem *item = (AnimationItem *)currentItem->data(0,1).value<void *>();
            if(item)
                emit itemSelectionChanged(item);
        }
    }
}

QTreeWidgetItem *Timeline::search(AnimationItem *item)
{
    for(int i=0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *twi = m_tree->topLevelItem(i);
        if(twi->data(0, 1).value<void *>() == item)
            return twi;
    }
    return nullptr;
}

QTreeWidgetItem *Timeline::search(QTreeWidgetItem *treeItem, QString propertyName)
{
    for(int i=0; i < treeItem->childCount(); i++)
    {
        QTreeWidgetItem *twi = treeItem->child(i);
        if(twi->text(0) == propertyName)
            return twi;
    }
    return nullptr;
}

void Timeline::idChanged(AnimationItem *item, QString value)
{
    QTreeWidgetItem *treeItem = search(item);
    if(treeItem)
    {
        treeItem->setText(0, value);
    }
}

void Timeline::deleteKeyFrameSlot(AnimationItem *item, QString propertyName, KeyFrame *frame)
{
    QUndoCommand *cmd = new DeleteKeyframeCommand(propertyName, frame, item, this);
    m_scene->undoStack()->push(cmd);
}

void Timeline::deleteTransitionSlot(AnimationItem *item, QString propertyName, KeyFrame *frame)
{
    QUndoCommand *cmd = new DeleteTransitionCommand(propertyName, frame, item, this);
    m_scene->undoStack()->push(cmd);
}

void Timeline::addTransitionSlot(AnimationItem *item, QString propertyName, KeyFrame *frame)
{
    QUndoCommand *cmd = new AddTransitionCommand(propertyName, frame, item, this);
    m_scene->undoStack()->push(cmd);
}

void Timeline::moveKeyframe(KeyFrame *key, int time)
{
    key->setTime(time);
    m_transitionPanel->keyframeMoved(key);
}

void Timeline::moveTransition(KeyFrame *key, int time)
{
    key->next()->setTime(key->next()->time() - key->time() + time);
    key->setTime(time);
    if(key->next()->easing() > -1)
        m_transitionPanel->transitionResized(key->next());
    if(key->prev() && key->prev()->easing() > -1)
        m_transitionPanel->transitionResized(key->prev());
    m_transitionPanel->transitionMoved(key);
}

void Timeline::resizeTransition(KeyFrame *key, int startTime, int endTime)
{
    key->next()->setTime(endTime);
    key->setTime(startTime);
    m_transitionPanel->transitionResized(key);
}

void Timeline::addTransition(AnimationItem *item, QString propertyName, KeyFrame *frame)
{
    frame->setEasing((int)QEasingCurve::Linear);
    m_transitionPanel->addTransition(item, propertyName, frame);
}

void Timeline::deleteTransition(AnimationItem *item, QString propertyName, KeyFrame *frame)
{
    frame->setEasing(-1);
    m_transitionPanel->deleteTransition(item, propertyName, frame);
}

void Timeline::deleteKeyFrame(AnimationItem *item, QString propertyName, KeyFrame *frame)
{
    if(item->deleteKeyframe(propertyName, frame))
    {
        QTreeWidgetItem *treeItem = search(item);
        if(treeItem)
        {
            QTreeWidgetItem *treeChildItem = search(treeItem, propertyName);
            if(treeChildItem)
            {
                treeItem->removeChild(treeChildItem);
                delete treeChildItem;

                if(treeItem->childCount() == 0)
                {
                    int index = m_tree->indexOfTopLevelItem(treeItem);
                    m_tree->takeTopLevelItem(index);
                    delete treeItem;
                }
            }
        }
    }
    emit propertyKeyRemoved(item, propertyName, frame);
    emit keyframeDeleted(item, propertyName);
}

void Timeline::addKeyFrame(AnimationItem *item, QString propertyName, QVariant value)
{
    KeyFrame *keyframe = new KeyFrame();
    keyframe->setValue(value);
    keyframe->setTime(m_playhead->value());

    QUndoCommand *cmd = new AddKeyframeCommand(propertyName, keyframe, item, this);
    item->scene()->undoStack()->push(cmd);
}

void Timeline::addKeyFrame(AnimationItem *item, QString propertyName, KeyFrame *frame)
{
    QTreeWidgetItem *treeChildItem = nullptr;


    QTreeWidgetItem *treeItem = search(item);
    if(treeItem)
    {
        treeChildItem = search(treeItem, propertyName);
    }
    else
    {
        treeItem = new QTreeWidgetItem();
        treeItem->setText(0, item->id());
        treeItem->setData(0, 1, qVariantFromValue((void *) item));
        treeItem->setData(1, 0, 1);
        connect(item, SIGNAL(idChanged(AnimationItem *, QString)), this, SLOT(idChanged(AnimationItem *, QString)));
        m_tree->addTopLevelItem(treeItem);
        emit lineAdded(item);
    }

    item->addKeyframe(propertyName, frame);
    if(treeChildItem)
    {
        QVariant var = treeChildItem->data(0, 1);
        QList<KeyFrame*> *list = (QList<KeyFrame*>*) var.value<void *>();
        list->append(frame);
        emit propertyKeyAdded(item, propertyName, frame);
    }
    else
    {
        QList<KeyFrame*> *list = new QList<KeyFrame*>();
        list->append(frame);
        treeChildItem = new QTreeWidgetItem();
        treeChildItem->setText(0, propertyName);
        treeChildItem->setData(0, 1, qVariantFromValue((void *) list));
        treeChildItem->setData(1, 0, 2);
        treeItem->addChild(treeChildItem);
        emit propertyAdded(item, propertyName);
        emit propertyKeyAdded(item, propertyName, frame);
    }
    treeItem->setExpanded(true);
}

void Timeline::keyframeAdded(AnimationItem * item, QString propertyName, KeyFrame *key)
{
    QTreeWidgetItem *treeChildItem = nullptr;

    QTreeWidgetItem *treeItem = search(item);
    if(treeItem)
    {
        treeChildItem = search(treeItem, propertyName);
    }
    else
    {
        treeItem = new QTreeWidgetItem();
        treeItem->setText(0, item->id());
        treeItem->setData(0, 1, qVariantFromValue((void *) item));
        treeItem->setData(1, 0, 1);
        connect(item, SIGNAL(idChanged(AnimationItem *, QString)), this, SLOT(idChanged(AnimationItem *, QString)));
        m_tree->addTopLevelItem(treeItem);
        emit lineAdded(item);
    }

    if(treeChildItem)
    {
        QVariant var = treeChildItem->data(0, 1);
        QList<KeyFrame*> *list = (QList<KeyFrame*>*) var.value<void *>();
        list->append(key);
        emit propertyKeyAdded(item, propertyName, key);
    }
    else
    {
        QList<KeyFrame*> *list = new QList<KeyFrame*>();
        list->append(key);
        treeChildItem = new QTreeWidgetItem();
        treeChildItem->setText(0, propertyName);
        treeChildItem->setData(0, 1, qVariantFromValue((void *) list));
        treeChildItem->setData(1, 0, 2);
        treeItem->addChild(treeChildItem);
        emit propertyAdded(item, propertyName);
        emit propertyKeyAdded(item, propertyName, key);
    }
    treeItem->setExpanded(true);
}


void Timeline::autokeyframes(bool value)
{
    m_scene->setAutokeyframes(value);
}

void Timeline::autotransitions(bool value)
{
    m_scene->setAutotransition(value);
}

void Timeline::removeItem(AnimationItem *item)
{
    for(int i=0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *treeItem = m_tree->topLevelItem(i);
        if(treeItem->data(0, 1).value<void *>() == item)
        {
            m_tree->takeTopLevelItem(i);
            delete treeItem;
        }
    }
    m_transitionPanel->removeItem(item);
}

void Timeline::selectItem(AnimationItem *item)
{
    for(int i=0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *treeItem = m_tree->topLevelItem(i);
        if(treeItem->data(0, 1).value<void *>() == item)
            treeItem->setSelected(true);
        else
            treeItem->setSelected(false);
    }
}

int Timeline::lastKeyframe()
{
    int last = 0;
    for(int i = 0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *treeItem = m_tree->topLevelItem(i);
        AnimationItem *item = (AnimationItem *) treeItem->data(0, 1).value<void *>();

        QHash<QString, KeyFrame*>::iterator it;
        for (it = item->keyframes()->begin(); it != item->keyframes()->end(); it++)
        {
            KeyFrame *first = it.value();
            for(KeyFrame *frame = first; frame != NULL; frame = frame->next())
            {
                last = frame->time() > last ? frame->time() : last;
            }
        }
    }
    return last;
}
