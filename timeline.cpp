#include "timeline.h"
#include "resizeableitem.h"
#include <QHeaderView>
#include <QLabel>
#include <QGridLayout>
#include <QMenu>
#include <QToolButton>
#include <QPropertyAnimation>
#include <QTest>

Timeline::Timeline()
    : QWidget(0)
{
    QVBoxLayout *vbox = new QVBoxLayout();
    QToolButton *playButton = new QToolButton();
    QAction *playAct = new QAction("Play");
    playAct->setIcon(QIcon(":/images/play.png"));
    playAct->setToolTip("Start the animation");
    connect(playAct, SIGNAL(triggered()), this, SLOT(playAnimation()));
    playButton->setDefaultAction(playAct);
    vbox->addWidget(playButton);
    QGridLayout *layout = new QGridLayout;
    m_timelineModel = new TimelineModel();
    m_treeview = new QTreeView(this);
    m_treeview->setModel(m_timelineModel);
    m_treeview->header()->close();


    QLabel *label2 = new QLabel("TIMELINE..................................");
    layout->addItem(vbox, 0, 0);
    layout->addWidget(m_treeview, 1, 0);
    layout->addWidget(label2, 1, 1);

    this->setLayout(layout);

    m_contextMenu = new QMenu();
    m_propertiesMenu = new QMenu("Add Property");
    m_contextMenu->addMenu(m_propertiesMenu);

    m_treeview->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    QItemSelectionModel *selectionModel = m_treeview->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(selectionChanged(const QItemSelection&,const QItemSelection&)));
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

    m_timelineModel->getAnimations()->start();
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
