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
    QGridLayout *layout = new QGridLayout;
    m_timelineModel = new TimelineModel();
    m_treeview = new QTreeView(this);
    m_treeview->setModel(m_timelineModel);
    m_treeview->header()->close();
    m_treeview->setStyleSheet("QTreeView::item:has-children:!selected {background-color: #4c4e50;} QTreeView::item:!selected { border-bottom: 1px solid #292929;} QTreeView::branch:!selected {border-bottom: 1px solid #292929;} QTreeView::branch:has-children:!selected {background-color: #4c4e50;} QTreeView::branch:has-children:!has-siblings:closed, QTreeView::branch:closed:has-children:has-siblings {border-image: none; image: url(:/images/branch-closed.png);} QTreeView::branch:open:has-children:!has-siblings, QTreeView::branch:open:has-children:has-siblings {border-image: none;image: url(:/images/branch-open.png);}");
    m_transitionPanel = new TransitionPanel();
    m_transitionPanel->setModel(m_timelineModel);
    m_transitionPanel->setTreeview(m_treeview);
    m_playhead = new PlayHead(Qt::Horizontal);
    m_playhead->setMinimum(0);
    m_playhead->setMaximum(m_playhead->width() * 5);
    m_playhead->setValue(0);
    m_playhead->setSingleStep(100);
    m_playhead->installEventFilter(this);

    layout->addItem(hbox, 0, 0);
    layout->addWidget(m_playhead, 0, 1);
    layout->addWidget(m_treeview, 1, 0);
    layout->addWidget(m_transitionPanel, 1, 1);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,1);

    this->setLayout(layout);

    m_contextMenu = new QMenu();
    m_delAct = new QAction("Delete");
    connect(m_delAct, SIGNAL(triggered(bool)), this, SLOT(deleteAnimation()));

    //m_contextMenu->addMenu(m_propertiesMenu);

    m_treeview->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    connect(m_treeview, SIGNAL(expanded(QModelIndex)), m_transitionPanel, SLOT(treeExpanded(QModelIndex)));
    connect(m_treeview, SIGNAL(collapsed(QModelIndex)), m_transitionPanel, SLOT(treeCollapsed(QModelIndex)));
    connect(this, SIGNAL(itemAdded()), m_transitionPanel, SLOT(treeItemAdded()));
    connect(m_treeview->verticalScrollBar(), SIGNAL(valueChanged(int)), m_transitionPanel, SLOT(treeScrollValueChanged(int)));

    connect(m_playhead, SIGNAL(valueChanged(int)), this, SLOT(playheadValueChanged(int)));

    QItemSelectionModel *selectionModel = m_treeview->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(selectionChanged(const QItemSelection&,const QItemSelection&)));

    connect(scene, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)), this, SLOT(addPropertyAnimation(ResizeableItem *, const QString, qreal, int, int)));
    connect(scene, SIGNAL(animationAdded(ResizeableItem *, QPropertyAnimation *)), this, SLOT(animationAdded(ResizeableItem *, QPropertyAnimation *)));
    connect(m_playhead, SIGNAL(valueChanged(int)), scene, SLOT(setPlayheadPosition(int)));
    connect(m_playhead, SIGNAL(sliderMoved(int)), this, SLOT(playheadMoved(int)));
}

void Timeline::reset()
{
    m_parallelAnimations = NULL;
    m_timelineModel->reset();
    m_transitionPanel->update();
}

bool Timeline::eventFilter(QObject *watched, QEvent *event)
{
    PlayHead * ph = dynamic_cast<PlayHead *>(watched);
    if (ph == NULL)
        return false;

    QResizeEvent * revent = dynamic_cast<QResizeEvent*>(event);
    if (revent == NULL)
        return false;

    m_playhead->setMaximum(m_playhead->width() * 5);
    return false;
}

void Timeline::animationChanged()
{
    m_transitionPanel->update();
    m_parallelAnimations = NULL;
}

void Timeline::animationAdded(ResizeableItem *item, QPropertyAnimation *anim)
{
    m_timelineModel->addAnimation(item, anim);
    m_parallelAnimations = NULL;
    emit itemAdded();
}

void Timeline::addPropertyAnimation(ResizeableItem *item, const QString propertyName, qreal value, int min, int max)
{
    m_timelineModel->addPropertyAnimation(item, propertyName, value, min, max);
    m_parallelAnimations = NULL;
    emit itemAdded();
    //m_treeview->setExpanded(index, true);
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

QParallelAnimationGroup *Timeline::getAnimations()
{
   createAnimationGroup();
   return m_parallelAnimations;
}

void Timeline::createAnimationGroup()
{
    if(m_parallelAnimations != NULL)
        return;
    m_parallelAnimations = new QParallelAnimationGroup();
    QList<QPropertyAnimation*> *anims = m_timelineModel->getAnimations();
    for(int i=0; i < anims->count() ; i++)
    {
        QPropertyAnimation *anim = anims->at(i);
        int begin = anim->property("begin").toInt();
        if(begin > 0)
        {
            QSequentialAnimationGroup *sag = new QSequentialAnimationGroup();
            QPropertyAnimation *pa = new QPropertyAnimation();
            pa->setDuration(begin);
            pa->setTargetObject(anim->targetObject());
            pa->setPropertyName(anim->propertyName());
            pa->setStartValue(anim->startValue());
            pa->setEndValue(anim->startValue());
            sag->addAnimation(pa);
            sag->addAnimation(anim);
            m_parallelAnimations->addAnimation(sag);
        }
        else
            m_parallelAnimations->addAnimation(anim);
    }
}

void Timeline::playAnimation()
{
    m_scene->clearSelection();

    createAnimationGroup();
    if(m_parallelAnimations->totalDuration() == 0)
        return;

    disconnect(m_playhead, SIGNAL(valueChanged(int)), this, SLOT(playheadValueChanged(int)));

    int delay = 1000 / m_scene->fps();
    int frames = m_parallelAnimations->totalDuration() / delay;

    m_parallelAnimations->start();
    m_parallelAnimations->pause();
    for(int i=0; i < frames; i++)
    {
        m_parallelAnimations->setCurrentTime(i * delay);
        m_playhead->setValue(i * delay);

        QTest::qSleep(delay / 2);
        QCoreApplication::processEvents(QEventLoop::AllEvents, delay / 2);
    }
    m_parallelAnimations->setCurrentTime(m_parallelAnimations->totalDuration());
    m_playhead->setValue(m_parallelAnimations->totalDuration());
    m_parallelAnimations->stop();

    connect(m_playhead, SIGNAL(valueChanged(int)), this, SLOT(playheadValueChanged(int)));
}

void Timeline::revertAnimation()
{
    m_scene->clearSelection();

    createAnimationGroup();
    if(m_parallelAnimations->totalDuration() == 0)
        return;

    disconnect(m_playhead, SIGNAL(valueChanged(int)), this, SLOT(playheadValueChanged(int)));
    m_parallelAnimations->start();
    m_parallelAnimations->pause();
    m_parallelAnimations->setCurrentTime(0);
    m_playhead->setValue(0);
    m_parallelAnimations->stop();

    connect(m_playhead, SIGNAL(valueChanged(int)), this, SLOT(playheadValueChanged(int)));
}

void Timeline::forwardAnimation()
{
    m_scene->clearSelection();

    createAnimationGroup();
    if(m_parallelAnimations->totalDuration() == 0)
        return;

    disconnect(m_playhead, SIGNAL(valueChanged(int)), this, SLOT(playheadValueChanged(int)));
    m_parallelAnimations->start();
    m_parallelAnimations->pause();
    m_parallelAnimations->setCurrentTime(m_parallelAnimations->totalDuration());
    m_playhead->setValue(m_parallelAnimations->totalDuration());
    m_parallelAnimations->stop();

    connect(m_playhead, SIGNAL(valueChanged(int)), this, SLOT(playheadValueChanged(int)));
}

bool compareKeyframes (KeyFrame *a, KeyFrame *b)
{
    return a->propertyName() < b->propertyName() || a->time() < b->time();
}

void Timeline::playheadMoved(int val)
{
    m_scene->clearSelection();

    for(int i=0; i < m_scene->items().count(); i++)
    {
        ResizeableItem *item = dynamic_cast<ResizeableItem *>(m_scene->items().at(i));
        if(item)
        {
            std::sort(item->keyframes()->begin(), item->keyframes()->end(), compareKeyframes);
            for(int j=0; j < item->keyframes()->count(); j++)
            {
                KeyFrame *key = item->keyframes()->at(j);
                if(key->time() <= val)
                {
                    if(key->propertyName() == "left")
                    {
                        item->setX(key->value().toReal());
                    }
                    else if(key->propertyName() == "top")
                    {
                        item->setY(key->value().toReal());
                    }
                } 
            }
        }
    }
}

void Timeline::playheadValueChanged(int val)
{
    /*
    m_scene->clearSelection();

    createAnimationGroup();
    if(m_parallelAnimations->totalDuration() == 0)
        return;

    m_parallelAnimations->start();
    m_parallelAnimations->pause();
    m_parallelAnimations->setCurrentTime(val);
    m_parallelAnimations->stop();
    */
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
        if(level == 2)
        {
//            QPropertyAnimation *anim = (QPropertyAnimation *) v.value<void *>();
//            if(anim)
//                emit animationSelectionChanged(anim);
        }
    }
}

void Timeline::deleteAnimation()
{
    qDebug() << "todo delete animation";
}

void Timeline::addKeyFrame(ResizeableItem *item, QString property, qreal value)
{
    m_timelineModel->addKeyFrame(item, property, value, m_playhead->value());
    emit itemAdded();
}
