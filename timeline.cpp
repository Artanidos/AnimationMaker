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
#include "resizeableitem.h"
#include "keyframe.h"

#include <QHeaderView>
#include <QLabel>
#include <QGridLayout>
#include <QMenu>
#include <QToolButton>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <QTest>
#include <QScrollBar>
#include <QTimeLine>

Timeline::Timeline(AnimationScene *scene)
    : QWidget(0)
{
    m_scene = scene;

    QHBoxLayout *hbox = new QHBoxLayout();
    QToolButton *playButton = new QToolButton();
    QToolButton *revertButton = new QToolButton();
    QToolButton *forwardButton = new QToolButton();
    m_time = new QLabel();
    m_time->setText("0:00.000");

    QAction *playAct = new QAction("Play");
    playAct->setIcon(QIcon(":/images/play.png"));
    playAct->setToolTip("Start the animation");
    connect(playAct, SIGNAL(triggered()), this, SLOT(playAnimation()));

    QAction *reverseAct = new QAction("Reverse");
    reverseAct->setIcon(QIcon(":/images/reverse.png"));
    reverseAct->setToolTip("Reset the animation");
    connect(reverseAct, SIGNAL(triggered()), this, SLOT(revertAnimation()));

    QAction *forwardAct = new QAction("Forward");
    forwardAct->setIcon(QIcon(":/images/forward.png"));
    forwardAct->setToolTip("Forward the animation");
    connect(forwardAct, SIGNAL(triggered()), this, SLOT(forwardAnimation()));

    revertButton->setDefaultAction(reverseAct);
    playButton->setDefaultAction(playAct);
    forwardButton->setDefaultAction(forwardAct);
    hbox->addWidget(revertButton);
    hbox->addWidget(playButton);
    hbox->addWidget(forwardButton);
    hbox->addStretch();
    hbox->addWidget(m_time);
    QGridLayout *layout = new QGridLayout;
    m_timelineModel = new TimelineModel();
    m_treeview = new QTreeView(this);
    m_treeview->setModel(m_timelineModel);
    m_treeview->header()->close();
    m_treeview->setStyleSheet("QTreeView::item:has-children:!selected {background-color: #4c4e50;} QTreeView::item:!selected { border-bottom: 1px solid #292929;} QTreeView::branch:!selected {border-bottom: 1px solid #292929;} QTreeView::branch:has-children:!selected {background-color: #4c4e50;} QTreeView::branch:has-children:!has-siblings:closed, QTreeView::branch:closed:has-children:has-siblings {border-image: none; image: url(:/images/branch-closed.png);} QTreeView::branch:open:has-children:!has-siblings, QTreeView::branch:open:has-children:has-siblings {border-image: none;image: url(:/images/branch-open.png);}");
    m_transitionPanel = new TransitionPanel();
    m_transitionPanel->setModel(m_timelineModel);
    m_transitionPanel->setTreeview(m_treeview);
    m_transitionPanel->setContextMenuPolicy(Qt::CustomContextMenu);
    m_playhead = new PlayHead();

    layout->addItem(hbox, 0, 0);
    layout->addWidget(m_playhead, 0, 1);
    layout->addWidget(m_treeview, 1, 0);
    layout->addWidget(m_transitionPanel, 1, 1);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,1);

    this->setLayout(layout);

    m_contextMenu = new QMenu();
    m_delAct = new QAction("Delete");

    m_transitionAct = new QAction("Add transition");

    //m_contextMenu->addMenu(m_propertiesMenu);

    m_treeview->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    connect(m_treeview, SIGNAL(expanded(QModelIndex)), m_transitionPanel, SLOT(treeExpanded(QModelIndex)));
    connect(m_treeview, SIGNAL(collapsed(QModelIndex)), m_transitionPanel, SLOT(treeCollapsed(QModelIndex)));
    connect(m_timelineModel, SIGNAL(keyframeAdded(ResizeableItem*,QString)), m_transitionPanel, SLOT(keyframeAdded(ResizeableItem *, QString)));

    connect(m_treeview->verticalScrollBar(), SIGNAL(valueChanged(int)), m_transitionPanel, SLOT(treeScrollValueChanged(int)));

    connect(m_playhead, SIGNAL(valueChanged(int)), this, SLOT(playheadValueChanged(int)));

    connect(m_transitionPanel, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onTransitionPanelContextMenu(const QPoint &)));
    connect(m_transitionAct, SIGNAL(triggered(bool)) ,this, SLOT(addTransition()));

    QItemSelectionModel *selectionModel = m_treeview->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(selectionChanged(const QItemSelection&,const QItemSelection&)));
    connect(m_playhead, SIGNAL(valueChanged(int)), scene, SLOT(setPlayheadPosition(int)));
    connect(m_scene, SIGNAL(keyframeAdded(ResizeableItem*, QString, KeyFrame*)), m_timelineModel, SLOT(keyframeAdded(ResizeableItem*, QString, KeyFrame*)));
}

void Timeline::reset()
{
    m_timelineModel->reset();
    m_transitionPanel->reset();
    m_playhead->setValue(0);
}

void Timeline::onCustomContextMenu(const QPoint &point)
{
    QModelIndex index = m_treeview->indexAt(point);
    if (index.isValid())
    {
        m_contextMenu->clear();
        m_contextMenu->addAction(m_delAct);
        m_contextMenu->exec(m_treeview->mapToGlobal(point));
    }
}

void Timeline::onTransitionPanelContextMenu(const QPoint &point)
{
    m_contextMenu->clear();
    m_contextMenu->addAction(m_transitionAct);
    m_contextMenu->exec(m_transitionPanel->mapToGlobal(point));
}

void Timeline::playAnimation()
{
    m_scene->clearSelection();

    int delay = 1000 / m_scene->fps();
    int last = m_timelineModel->lastKeyframe();
    int frames = last / delay;

    for(int i=0; i < frames; i++)
    {
        m_playhead->setValue(i * delay);
        QTest::qSleep(delay / 2);
        QCoreApplication::processEvents(QEventLoop::AllEvents, delay / 2);
    }
    m_playhead->setValue(m_timelineModel->lastKeyframe());
}

void Timeline::revertAnimation()
{
    m_scene->clearSelection();
    m_playhead->setValue(0);
}

void Timeline::forwardAnimation()
{
    m_scene->clearSelection();

    int last = m_timelineModel->lastKeyframe();
    m_playhead->setValue(last);
}

void Timeline::playheadValueChanged(int val)
{
    int seconds = val / 1000;
    int minutes = seconds / 60;
    int milliseconds = val - seconds * 1000;
    QString txt;
    txt.sprintf("%d:%02d.%03d", minutes, seconds, milliseconds);
    m_time->setText(txt);
    m_scene->clearSelection();
    m_scene->setPlayheadPosition(val);
}

void Timeline::selectionChanged(const QItemSelection& current,const QItemSelection&)
{
    if(current.count() && current.at(0).indexes().count())
    {
        const QModelIndex index = current.at(0).indexes().at(0);
        QVariant v = index.data(Qt::UserRole);
        QVariant level = index.data(Qt::UserRole + 1);

        if(level == 1)
        {
            ResizeableItem *item = (ResizeableItem *) v.value<void *>();
            if(item)
                emit itemSelectionChanged(item);
        }
    }
}

void Timeline::addKeyFrame(ResizeableItem *item, QString propertyName, qreal value)
{
    m_timelineModel->addKeyFrame(item, propertyName, value, m_playhead->value());
}

void Timeline::addTransition()
{
    ResizeableItem *item = dynamic_cast<ResizeableItem *>(m_scene->items().at(0));
    if(item)
    {
        QList<KeyFrame*> *list = item->keyframes()->value("left");
        KeyFrame *first = list->first();
        KeyFrame *last = list->last();
        first->setEasing((int)QEasingCurve::OutBounce);
        first->setTransitionTo(last);
        qDebug() << "added transition";
    }
}
