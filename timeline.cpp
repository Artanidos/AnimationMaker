#include "timeline.h"
#include "resizeableitem.h"
#include <QHeaderView>
#include <QLabel>
#include <QGridLayout>
#include <QMenu>
#include <QToolButton>
#include <QPropertyAnimation>
#include <QTest>

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
    m_treeview->setFixedWidth(300);

    QWidget *time = new QWidget();
    time->setStyleSheet("background-color: #4c4e50");
    time->setMinimumWidth(300);
    time->setMinimumHeight(50);
    layout->addItem(hbox, 0, 0);
    layout->addWidget(m_treeview, 1, 0);
    layout->addWidget(time, 1, 1);

    this->setLayout(layout);

    m_contextMenu = new QMenu();
    m_propertiesMenu = new QMenu("Add Property");
    m_contextMenu->addMenu(m_propertiesMenu);

    m_treeview->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    QItemSelectionModel *selectionModel = m_treeview->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(selectionChanged(const QItemSelection&,const QItemSelection&)));

    connect(scene, SIGNAL(addPropertyAnimation(ResizeableItem *, const QString, qreal)), this, SLOT(addPropertyAnimation(ResizeableItem *, const QString, qreal)));
}

void Timeline::addPropertyAnimation(ResizeableItem *item, const QString propertyName, qreal value)
{
    m_timelineModel->addPropertyAnimation(item, propertyName, value);
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

void Timeline::playAnimation()
{
    //emit playAnimationPressed();
    m_scene->clearSelection();
    m_timelineModel->getAnimations()->start();
}

void Timeline::revertAnimation()
{
    m_scene->clearSelection();
    m_timelineModel->getAnimations()->start();
    m_timelineModel->getAnimations()->pause();
    m_timelineModel->getAnimations()->setCurrentTime(0);
}

void Timeline::forwardAnimation()
{
    m_scene->clearSelection();
    m_timelineModel->getAnimations()->start();
    m_timelineModel->getAnimations()->pause();
    m_timelineModel->getAnimations()->setCurrentTime(m_timelineModel->getAnimations()->totalDuration());
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
