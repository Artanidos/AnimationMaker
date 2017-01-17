#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ellipse.h"
#include "treemodel.h"

#include <QtTest/QTest>
#include <QMessageBox>


void video_encode(const char *, QQuickView *);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    registerTypes();
    createStatusBar();
    createActions();
    createMenus();
    createGui();
    readSettings();


}

MainWindow::~MainWindow()
{
    delete view;
    delete container;
    delete scroll;
    delete tree;
    delete splitter;   
}

void MainWindow::createGui()
{
    view = new QQuickView;
    view->setSource(QUrl::fromLocalFile("/home/olaf/SourceCode/AnimationMaker/demo.qml"));
    model = new TreeModel(view->rootObject());
    QSize size = view->size();
    view->setMinimumSize(size);
    tree = new QTreeView();
    tree->setModel(model);
    tree->header()->close();
    scroll = new QScrollArea();
    container = QWidget::createWindowContainer(view);
    container->setMaximumSize(size);
    container->setMinimumSize(size);
    scroll->setWidgetResizable(true);
    scroll->setWidget(container);
    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(tree);
    splitter->addWidget(scroll);
    splitter->setStretchFactor(0,0);
    splitter->setStretchFactor(1,1);
    setCentralWidget(splitter);
}

void MainWindow::registerTypes()
{
    qmlRegisterType<EllipseBorder>();
    qmlRegisterType<Ellipse>("CrowdWare", 1, 0, "Ellipse");
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
        writeSettings();
        event->accept();
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
        restoreState(settings.value("state").toByteArray());
    }
}

void MainWindow::exportAnimation()
{
    if(view) delete view;
    view = new QQuickView;
    view->setSource(QUrl::fromLocalFile("/home/olaf/SourceCode/AnimationMaker/demo.qml"));
    view->setResizeMode( QQuickView::SizeViewToRootObject );
    view->create();
    video_encode("test.mpg", view);
}

void MainWindow::playAnimation()
{
    int duration = 0;
    int fps = 25;

    QObject *obj = view->rootObject();
    QObject *ac = obj->findChild<QObject*>("animationController");
    if(ac)
    {
        QObject *animation = ac->findChild<QObject*>();
        if(strcmp(animation->metaObject()->className(), "QQuickParallelAnimation") == 0 )
        {
            foreach(QObject *a, animation->children())
            {
                duration = std::max(a->property("duration").toInt(), duration);
            }
        }

        int frames = duration/ fps;

        for (int i = 0; i < frames; i++)
        {
            ac->setProperty("progress", 1.0/frames * i);
            QTest::qSleep(40);
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
        }
    }

}

void MainWindow::createActions()
{
    toolbar = new QToolBar("Play");
    toolbar->setObjectName("PlayToolbar");
    addToolBar(toolbar);
    playAct = new QAction(tr("&Play..."), this);
    playAct->setIcon(QIcon(":/images/play.png"));
    connect(playAct, SIGNAL(triggered()), this, SLOT(playAnimation()));
    toolbar->addAction(playAct);

    saveAsAct = new QAction(tr("&Export"), this);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(exportAnimation()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(playAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("AnimationMaker"),
            tr("The AnimationMaker is a tool to create presentation videos."));
}
