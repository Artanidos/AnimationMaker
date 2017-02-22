#include "timeline.h"
#include "resizeableitem.h"

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
    qreal max = (qreal)m_playhead->width() / m_scene->fps() * 5;
    m_playhead->setMaximum(max * 100);
    m_playhead->setValue(0);
    m_playhead->installEventFilter(this);

    layout->addItem(hbox, 0, 0);
    layout->addWidget(m_playhead, 0, 1);
    layout->addWidget(m_treeview, 1, 0);
    layout->addWidget(m_transitionPanel, 1, 1);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,1);

    this->setLayout(layout);

    m_contextMenu = new QMenu();
    m_propertiesMenu = new QMenu("Add Property");
    m_contextMenu->addMenu(m_propertiesMenu);

    m_treeview->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    connect(m_treeview, SIGNAL(expanded(QModelIndex)), m_transitionPanel, SLOT(treeExpanded(QModelIndex)));
    connect(m_treeview, SIGNAL(collapsed(QModelIndex)), m_transitionPanel, SLOT(treeCollapsed(QModelIndex)));
    connect(this, SIGNAL(itemAdded()), m_transitionPanel, SLOT(treeItemAdded()));
    connect(m_treeview->verticalScrollBar(), SIGNAL(valueChanged(int)), m_transitionPanel, SLOT(treeScrollValueChanged(int)));

    connect(m_playhead, SIGNAL(valueChanged(int)), this, SLOT(playheadValueChanged(int)));

    QItemSelectionModel *selectionModel = m_treeview->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(selectionChanged(const QItemSelection&,const QItemSelection&)));

    connect(scene, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal)), this, SLOT(addPropertyAnimation(ResizeableItem *, const QString, qreal)));
    connect(scene, SIGNAL(animationAdded(ResizeableItem *, QPropertyAnimation *)), this, SLOT(animationAdded(ResizeableItem *, QPropertyAnimation *)));
}

bool Timeline::eventFilter(QObject *watched, QEvent *event)
{
    PlayHead * ph = dynamic_cast<PlayHead *>(watched);
    if ( ph == NULL)
    {
        return false;
    }

    QResizeEvent * revent = dynamic_cast<QResizeEvent*>(event);
    if ( revent == NULL)
    {
        qreal max = (qreal)m_playhead->width() / m_scene->fps() * 5;
        m_playhead->setMaximum(max * 100);
        return false;
    }
    return false;
}

void Timeline::animationChanged()
{
    m_transitionPanel->update();
}

void Timeline::animationAdded(ResizeableItem *item, QPropertyAnimation *anim)
{
    m_timelineModel->addAnimation(item, anim);
    emit itemAdded();
}

void Timeline::addPropertyAnimation(ResizeableItem *item, const QString propertyName, qreal value)
{
    m_timelineModel->addPropertyAnimation(item, propertyName, value);
    emit itemAdded();
    //m_treeview->setExpanded(index, true);
}

void Timeline::onCustomContextMenu(const QPoint &point)
{
    QModelIndex index = m_treeview->indexAt(point);
    if (index.isValid())
    {
        m_propertiesMenu->clear();
        m_propertiesMenu->addAction(m_xAct);
        m_propertiesMenu->addAction(m_yAct);
        m_propertiesMenu->addAction(m_opacityAct);
        m_contextMenu->exec(m_treeview->mapToGlobal(point));
    }
}

QParallelAnimationGroup *getAnimationGroup(QList<QPropertyAnimation*> *anims)
{
    QParallelAnimationGroup *pag = new QParallelAnimationGroup();
    for(int i=0; i < anims->count() ; i++)
    {
        QPropertyAnimation *anim = anims->at(i);
        int begin = anim->property("begin").toInt();
        if(begin > 0)
        {
            QSequentialAnimationGroup *sag = new QSequentialAnimationGroup();
            QPauseAnimation *pa = new QPauseAnimation(begin);
            sag->addAnimation(pa);
            sag->addAnimation(anim);
            pag->addAnimation(sag);
        }
        else
            pag->addAnimation(anim);
    }
    return pag;
}

void Timeline::playAnimation()
{
    m_scene->clearSelection();

    QParallelAnimationGroup *pag = getAnimationGroup(m_timelineModel->getAnimations());

    qreal frames = (qreal)pag->totalDuration() / m_scene->fps();
    if(frames == 0)
        return;

    int delay = 1000 / frames;

    qreal max = (qreal)m_playhead->width() / m_scene->fps() * 5;
    m_playhead->setMaximum(max * 100);

    pag->start();
    pag->pause();
    for(int i=0; i < frames; i++)
    {
        pag->setCurrentTime(i * delay);
        m_playhead->setValue(i * 100);

        QTest::qSleep(delay);
        QCoreApplication::processEvents(QEventLoop::AllEvents, delay);
    }
    pag->setCurrentTime(pag->totalDuration());
    m_playhead->setValue(frames * 100);
}

void Timeline::revertAnimation()
{
    m_scene->clearSelection();
    QParallelAnimationGroup *pag = getAnimationGroup(m_timelineModel->getAnimations());
    pag->start();
    pag->pause();
    pag->setCurrentTime(0);
    m_playhead->setValue(0);
}

void Timeline::forwardAnimation()
{
    m_scene->clearSelection();
    QParallelAnimationGroup *pag = getAnimationGroup(m_timelineModel->getAnimations());
    pag->start();
    pag->pause();
    pag->setCurrentTime(pag->totalDuration());

    qreal frames = (qreal)pag->totalDuration() / m_scene->fps();
    qreal max = (qreal)m_playhead->width() / m_scene->fps() * 5;
    m_playhead->setMaximum(max * 100);
    m_playhead->setValue(frames * 100);
}

void Timeline::playheadValueChanged(int val)
{
    m_scene->clearSelection();
    QParallelAnimationGroup *pag = getAnimationGroup(m_timelineModel->getAnimations());
    pag->start();
    pag->pause();
    pag->setCurrentTime(val / 100 * m_scene->fps());
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
            QPropertyAnimation *anim = (QPropertyAnimation *) v.value<void *>();
            if(anim)
                emit animationSelectionChanged(anim);
        }
    }
}
