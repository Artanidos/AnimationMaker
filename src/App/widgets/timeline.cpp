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

#include <QEasingCurve>
#include <QGridLayout>
#include <QHash>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QList>
#include <QTest>

#include "commands.h"
#include "keyframehandle.h"
#include "transition.h"
#include "transitionline.h"

Timeline::Timeline(AnimationScene *scene)
    : QWidget(nullptr)
{
    m_scene = scene;
    m_horizontalScrollPos = 0;
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

    m_playAct = new QAction("Play");
    m_playAct->setIcon(QIcon(":/images/play.png"));
    m_playAct->setToolTip("Start the animation");
    connect(m_playAct, SIGNAL(triggered()), this, SLOT(playAnimation()));

    m_pauseAct = new QAction("Pause");
    m_pauseAct->setIcon(QIcon(":/images/pause.png"));
    m_pauseAct->setToolTip("Pause the animation");
    connect(m_pauseAct, SIGNAL(triggered()), this, SLOT(pauseAnimation()));

    m_reverseAct = new QAction("Reverse");
    m_reverseAct->setIcon(QIcon(":/images/reverse.png"));
    m_reverseAct->setToolTip("Reverse the animation");
    connect(m_reverseAct, SIGNAL(triggered()), this, SLOT(revertAnimation()));

    m_forwardAct = new QAction("Forward");
    m_forwardAct->setIcon(QIcon(":/images/forward.png"));
    m_forwardAct->setToolTip("Forward the animation");
    connect(m_forwardAct, SIGNAL(triggered()), this, SLOT(forwardAnimation()));

    revertButton->setDefaultAction(m_reverseAct);
    playButton->setDefaultAction(m_playAct);
    pauseButton->setDefaultAction(m_pauseAct);
    forwardButton->setDefaultAction(m_forwardAct);

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
    m_tree->setColumnCount(2);
    m_tree->header()->resizeSection(0, 205);
    m_tree->header()->close();
    m_tree->setStyleSheet("QTreeWidget::item:has-children:!selected {background-color: #4c4e50;} QTreeWidget::item:!selected { border-bottom: 1px solid #292929;} QTreeView::branch:!selected {border-bottom: 1px solid #292929;} QTreeWidget::branch:has-children:!selected {background-color: #4c4e50;} QTreeWidget::branch:has-children:!has-siblings:closed, QTreeWidget::branch:closed:has-children:has-siblings {border-image: none; image: url(:/images/branch-closed.png);} QTreeWidget::branch:open:has-children:!has-siblings, QTreeWidget::branch:open:has-children:has-siblings {border-image: none;image: url(:/images/branch-open.png);}");
    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);

    m_playhead = new PlayHead();
    m_sb = new QScrollBar(Qt::Horizontal);
    m_sb->setMaximum(50);
    layout->setColumnMinimumWidth(0, 200);
    layout->addItem(hbox, 0, 0);
    layout->addWidget(m_playhead, 0, 1);
    layout->addWidget(m_tree, 1, 0, 1, 2);
    layout->addWidget(m_sb, 2, 1);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,1);

    this->setLayout(layout);

    m_contextMenu = new QMenu();
    m_delAct = new QAction("Delete");

    connect(m_tree, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    connect(m_playhead, SIGNAL(valueChanged(int)), this, SLOT(playheadValueChanged(int)));
    connect(m_sb, SIGNAL(valueChanged(int)), m_playhead, SLOT(scrollValueChanged(int)));
    connect(m_sb, SIGNAL(valueChanged(int)), this, SLOT(scrollValueChanged(int)));
    connect(m_tree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(treeCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(m_playhead, SIGNAL(valueChanged(int)), scene, SLOT(setPlayheadPosition(int)));
    connect(m_scene, SIGNAL(keyframeAdded(AnimationItem*, QString, KeyFrame*)), this, SLOT(keyframeAdded(AnimationItem*, QString, KeyFrame*)));
}

Timeline::~Timeline()
{
    delete m_contextMenu;
    delete m_delAct;
    delete m_forwardAct;
    delete m_pauseAct;
    delete m_playAct;
    delete m_reverseAct;
    delete m_tree;
}

void Timeline::scrollValueChanged(int value)
{
    m_horizontalScrollPos = value;
    int max = m_sb->maximum();
    if(value > max * 90 / 100)
        m_sb->setMaximum(max * 110 / 100);

    for(int i = 0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *ti = m_tree->topLevelItem(i);
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_tree->itemWidget(ti, 1));
        line->setScrollValue(value);
        for(int j = 0; j < ti->childCount(); j++)
        {
            QTreeWidgetItem *tvi = ti->child(j);
            TransitionLine *cline = dynamic_cast<TransitionLine*>(m_tree->itemWidget(tvi, 1));
            cline->setScrollValue(value);
        }
    }
}

void Timeline::addTransitionLine(QTreeWidgetItem *tvi, AnimationItem *item)
{
    TransitionLine *line = new TransitionLine(item, "", this, this->m_scene->undoStack());
    line->setScrollValue(m_horizontalScrollPos);
    m_tree->setItemWidget(tvi, 1, line);
}

void Timeline::addProperty(QTreeWidgetItem *treeChildItem, AnimationItem *item, QString propertyName)
{
    TransitionLine *line = new TransitionLine(item, propertyName, this, m_scene->undoStack());
    line->setScrollValue(m_horizontalScrollPos);
    m_tree->setItemWidget(treeChildItem, 1, line);
    connect(line, SIGNAL(keyframeDeleted(AnimationItem*,QString,KeyFrame*)), this, SLOT(deleteKeyFrameSlot(AnimationItem*,QString,KeyFrame*)));
    connect(line, SIGNAL(deleteTransition(AnimationItem*,QString,KeyFrame*)), this, SLOT(deleteTransitionSlot(AnimationItem*,QString,KeyFrame*)));
    connect(line, SIGNAL(transitionAdded(AnimationItem*,QString,KeyFrame*)), this, SLOT(addTransitionSlot(AnimationItem*,QString,KeyFrame*)));
    connect(line, SIGNAL(transitionSelected(KeyFrame*)), this, SLOT(transitionSelected(KeyFrame*)));
}

void Timeline::addPropertyKey(QTreeWidgetItem *treeChildItem, AnimationItem *item, QString propertyName, KeyFrame *key)
{
    TransitionLine *line = dynamic_cast<TransitionLine*>(m_tree->itemWidget(treeChildItem, 1));
    line->addKeyframe(propertyName, key);
}

void Timeline::propertyKeyRemoved(AnimationItem *item, QString propertyName, KeyFrame *key)
{
    QTreeWidgetItem *tvi = search(item);
    if(tvi)
    {
        QTreeWidgetItem *ctvi = search(tvi, propertyName);
        if(ctvi)
        {
            TransitionLine *line = dynamic_cast<TransitionLine*>(m_tree->itemWidget(ctvi, 1));
            line->removeKeyframe(propertyName, key);
        }
    }
}

void Timeline::transitionResized(KeyFrame *key)
{
    for(int i = 0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *tvi = m_tree->topLevelItem(i);
        for(int j = 0; j < tvi->childCount(); j++)
        {
            QTreeWidgetItem *ctvi = tvi->child(j);
            TransitionLine *line = dynamic_cast<TransitionLine*>(m_tree->itemWidget(ctvi, 1));
            Transition *transition = line->getTransition(key);
            if(transition)
            {
                transition->resizeTransition();
                if(key->next()->easing() > -1)
                    line->getTransition(key->next())->resizeTransition();
                if(key->prev() && key->prev()->easing() > -1)
                    line->getTransition(key->prev())->resizeTransition();
                return;
            }
        }
    }
}

void Timeline::transitionMoved(KeyFrame *key)
{
    for(int i = 0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *tvi = m_tree->topLevelItem(i);
        for(int j = 0; j < tvi->childCount(); j++)
        {
            QTreeWidgetItem *ctvi = tvi->child(j);
            TransitionLine *line = dynamic_cast<TransitionLine*>(m_tree->itemWidget(ctvi, 1));
            Transition *transition = line->getTransition(key);
            if(transition)
            {
                transition->move(transition->key()->time() / 5 - line->horizontalScrollValue() * 20,0);
                return;
            }
        }
    }
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

QString Timeline::timeString(int val, bool showMinutes)
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
    m_time->setText(Timeline::timeString(val));
    m_scene->clearSelection();
    m_scene->setPlayheadPosition(val);

    for(int i=0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *treeItem = m_tree->topLevelItem(i);
        TransitionLine *line = dynamic_cast<TransitionLine*>(m_tree->itemWidget(treeItem, 1));
        line->setPlayheadPosition(val);

        for(int j = 0; j < treeItem->childCount(); j++)
        {
            QTreeWidgetItem *cTreeItem = treeItem->child(j);
            TransitionLine *line = dynamic_cast<TransitionLine*>(m_tree->itemWidget(cTreeItem, 1));
            line->setPlayheadPosition(val);
        }
    }
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

    for(int i = 0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *tvi = m_tree->topLevelItem(i);
        for(int j = 0; j < tvi->childCount(); j++)
        {
            QTreeWidgetItem *ctvi = tvi->child(j);
            TransitionLine *line = dynamic_cast<TransitionLine*>(m_tree->itemWidget(ctvi, 1));
            KeyframeHandle *handle = line->getKeyframeHandle(key);
            if(handle)
            {
                handle->move(handle->key()->time() / 5 - line->horizontalScrollValue() * 20 - 6, 2);
                break;
            }
        }
    }
}

void Timeline::moveTransition(KeyFrame *key, int time)
{
    key->next()->setTime(key->next()->time() - key->time() + time);
    key->setTime(time);
    if(key->next()->easing() > -1)
        transitionResized(key->next());
    if(key->prev() && key->prev()->easing() > -1)
        transitionResized(key->prev());
    transitionMoved(key);
}

void Timeline::resizeTransition(KeyFrame *key, int startTime, int endTime)
{
    key->next()->setTime(endTime);
    key->setTime(startTime);
    transitionResized(key);
}

TransitionLine *Timeline::getTransitionLine(AnimationItem *item, QString propertyName)
{
    QTreeWidgetItem *tvi = search(item);
    if(tvi)
    {
        QTreeWidgetItem *child = search(tvi, propertyName);
        if(child)
        {
            TransitionLine *line = dynamic_cast<TransitionLine*>(m_tree->itemWidget(child, 1));
            return line;
        }
    }
    return nullptr;
}

void Timeline::addTransition(AnimationItem *item, QString propertyName, KeyFrame *key)
{
    key->setEasing((int)QEasingCurve::Linear);
    QTreeWidgetItem *tvi = search(item);
    if(tvi)
    {
        QTreeWidgetItem *child = search(tvi, propertyName);
        if(child)
        {
            TransitionLine *line = dynamic_cast<TransitionLine*>(m_tree->itemWidget(child, 1));
            line->addTransitionGui(key);
        }
    }
}

void Timeline::deleteTransition(AnimationItem *item, QString propertyName, KeyFrame *key)
{
    key->setEasing(-1);
    QTreeWidgetItem *tvi = search(item);
    if(tvi)
    {
        QTreeWidgetItem *child = search(tvi, propertyName);
        if(child)
        {
            TransitionLine *line = dynamic_cast<TransitionLine*>(m_tree->itemWidget(child, 1));
            line->removeTransition(propertyName, key);
        }
    }
}

void Timeline::deleteKeyFrame(AnimationItem *item, QString propertyName, KeyFrame *key)
{
    if(item->deleteKeyframe(propertyName, key))
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
    propertyKeyRemoved(item, propertyName, key);
}

void Timeline::addKeyFrame(AnimationItem *item, QString propertyName, QVariant value)
{
    KeyFrame *keyframe = new KeyFrame();
    keyframe->setValue(value);
    keyframe->setTime(m_playhead->value());

    QUndoCommand *cmd = new AddKeyframeCommand(propertyName, keyframe, item, this);
    item->scene()->undoStack()->push(cmd);
}

void Timeline::addKeyFrame(AnimationItem *item, QString propertyName, KeyFrame *key)
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
        addTransitionLine(treeItem, item);
    }

    item->addKeyframe(propertyName, key);
    if(treeChildItem)
    {
        QVariantList list = treeChildItem->data(0, 1).toList();
        list.append(QVariant::fromValue(key));
        treeChildItem->setData(0, 1, list);
        addPropertyKey(treeChildItem, item, propertyName, key);
    }
    else
    {
        QList<KeyFrame*> list = QList<KeyFrame *>();
        list.append(key);
        treeChildItem = new QTreeWidgetItem();
        treeChildItem->setText(0, propertyName);
        treeChildItem->setData(0, 1, qVariantFromValue(list));
        treeChildItem->setData(1, 0, 2);
        treeItem->addChild(treeChildItem);

        addProperty(treeChildItem, item, propertyName);
        addPropertyKey(treeChildItem, item, propertyName, key);
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
        addTransitionLine(treeItem, item);
    }

    if(treeChildItem)
    {
        QVariantList list = treeChildItem->data(0, 1).toList();
        list.append(QVariant::fromValue(key));
        treeChildItem->setData(0, 1, list);
        addPropertyKey(treeChildItem, item, propertyName, key);
    }
    else
    {
        QList<KeyFrame*> list = QList<KeyFrame*>();
        list.append(key);
        treeChildItem = new QTreeWidgetItem();
        treeChildItem->setText(0, propertyName);
        treeChildItem->setData(0, 1, qVariantFromValue(list));
        treeChildItem->setData(1, 0, 2);
        treeItem->addChild(treeChildItem);
        addProperty(treeChildItem, item, propertyName);
        addPropertyKey(treeChildItem, item, propertyName, key);
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
