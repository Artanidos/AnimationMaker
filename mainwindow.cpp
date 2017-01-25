#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treemodel.h"
#include "animationscene.h"

#include <QtTest/QTest>
#include <QMessageBox>

void video_encode(const char *, QQuickView *);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setDockNestingEnabled(true);
    createStatusBar();
    createActions();
    createMenus();
    createGui();
    readSettings();
}

MainWindow::~MainWindow()
{
    delete tree;
    delete scene;
    delete view;
}

void MainWindow::save()
{
    writeFile(loadedFile.fileName());
}

void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Animation"), "", tr("AnimationMaker (*.amb);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    writeFile(fileName);
    loadedFile.setFile(fileName);
    saveAct->setEnabled(true);
}

void MainWindow::writeFile(QString fileName)
{
    scene->deselectAll();

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    // Write a header with a "magic number" and a version
    out << (quint32)0xA0B0C0D0;
    out << (qint32)123;
    out.setVersion(QDataStream::Qt_5_7);

    out << scene;

    file.close();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Animation"), "", tr("AnimationMaker (*.amb);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    // Read and check the header
    quint32 magic;
    in >> magic;
    if (magic != 0xA0B0C0D0)
    {
        file.close();
        printf("bad fileformat\n");
        return;
    }

    // Read the version
    qint32 version;
    in >> version;
    if (version < 100)
    {
        file.close();
        printf("file to old\n");
        return;
    }
    if (version > 123)
    {
        file.close();
        printf("file to new\n");
        return;
    }

    if (version <= 110)
        in.setVersion(QDataStream::Qt_4_0);
    else
        in.setVersion(QDataStream::Qt_5_7);

    // Read the data
    in >> scene;
    file.close();

    model->setScene(scene);

    tree->expandAll();
    loadedFile.setFile(fileName);
    saveAct->setEnabled(true);
    setWindowTitle(QCoreApplication::applicationName() + " - " + loadedFile.completeBaseName() + "." + loadedFile.suffix());
}

void MainWindow::createGui()
{
    QToolBar *toolpanel = new QToolBar();
    toolpanel->setOrientation(Qt::Vertical);
    QActionGroup *anActionGroup = new QActionGroup(toolpanel);
    selectAct = new QAction("Select", anActionGroup);
    selectAct->setIcon(QIcon(":/images/arrow.png"));
    rectangleAct = new QAction("Rectangle", anActionGroup);
    rectangleAct->setIcon(QIcon(":/images/rectangle.png"));
    ellipseAct = new QAction("Ellipse", anActionGroup);
    ellipseAct->setIcon(QIcon(":/images/ellipse.png"));
    selectAct->setCheckable(true);
    rectangleAct->setCheckable(true);
    ellipseAct->setCheckable(true);
    connect(selectAct, SIGNAL(triggered()), this, SLOT(setSelectMode()));
    connect(rectangleAct, SIGNAL(triggered()), this, SLOT(setRectangleMode()));
    connect(ellipseAct, SIGNAL(triggered()), this, SLOT(setEllipseMode()));
    toolpanel->addAction(selectAct);
    toolpanel->addAction(rectangleAct);
    toolpanel->addAction(ellipseAct);

    selectAct->toggle();

    QDockWidget *tooldock = new QDockWidget(tr("Tools"), this);
    tooldock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tooldock->setWidget(toolpanel);
    tooldock->setObjectName("Tools");
    addDockWidget(Qt::LeftDockWidgetArea, tooldock);

    QLabel *propertiespanel = new QLabel();
    propertiespanel->setMinimumWidth(320);
    QImage propertiesimage;
    propertiesimage.load("/home/olaf/SourceCode/AnimationMaker/properties.png");
    propertiespanel->setPixmap(QPixmap::fromImage(propertiesimage));
    propertiespanel->setAlignment(Qt::AlignTop);

    QDockWidget *propertiesdock = new QDockWidget(tr("Properties"), this);
    propertiesdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    propertiesdock->setWidget(propertiespanel);
    propertiesdock->setObjectName("Properties");
    addDockWidget(Qt::RightDockWidgetArea, propertiesdock);

    scene = new AnimationScene();
    scene->setSceneRect(QRect(0,0,1200,720));

    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::RenderHint::Antialiasing);
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(sceneSeletionChanged()));
    connect(scene, SIGNAL(itemAdded(QGraphicsItem*)), this, SLOT(sceneItemAdded(QGraphicsItem*)));
    //connect(scene, SIGNAL(itemAdded(QGraphicsItem*)), this, SLOT(sceneItemAdded(QGraphicsItem*)));

    model = new TreeModel();
    model->setScene(scene);
    tree = new QTreeView();
    tree->setModel(model);
    tree->header()->close();
    tree->expandAll();
    tree->setMinimumWidth(320);
    tree->setCurrentIndex(model->index(0, 0));
    QItemSelectionModel *selectionModel = tree->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(selectionChanged(const QItemSelection&,const QItemSelection&)));

    QDockWidget *elementsdock = new QDockWidget(tr("Elements"), this);
    elementsdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    elementsdock->setWidget(tree);
    elementsdock->setObjectName("Elements");
    addDockWidget(Qt::LeftDockWidgetArea, elementsdock);
    splitDockWidget(tooldock, elementsdock, Qt::Horizontal);

    timeline = new QLabel();
    timeline->setMinimumHeight(110);
    QImage timelineimage;
    timelineimage.load("/home/olaf/SourceCode/AnimationMaker/timeline.png");
    timeline->setPixmap(QPixmap::fromImage(timelineimage));
    timeline->setMinimumWidth(300);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);
    layout->addWidget(timeline);

    QWidget *w = new QWidget();
    w->setLayout(layout);

    setCentralWidget(w);
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
    /*
    if(view) delete view;
    view = new QQuickView;
    view->setSource(QUrl::fromLocalFile("/home/olaf/SourceCode/AnimationMaker/demo.qml"));
    view->setResizeMode( QQuickView::SizeViewToRootObject );
    view->create();
    video_encode("test.mpg", view);
    */
}

void MainWindow::playAnimation()
{
    /*
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
    */
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setEnabled(false);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    delAct = new QAction(tr("Delete"), this);
    delAct->setShortcut(tr("Delete"));
    connect(delAct, SIGNAL(triggered()), this, SLOT(deleteItem()));

    playAct = new QAction(tr("&Play"), this);
    playAct->setIcon(QIcon(":/images/play.png"));
    playAct->setToolTip("Start the animation");
    connect(playAct, SIGNAL(triggered()), this, SLOT(playAnimation()));

    exportAct = new QAction(tr("&Export"), this);
    connect(exportAct, SIGNAL(triggered()), this, SLOT(exportAnimation()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exportAct);
    fileMenu->addSeparator();
    fileMenu->addAction(playAct);
    fileMenu->addSeparator();
    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(delAct);
    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("AnimationMaker"),
            tr("The AnimationMaker is a tool to create presentation videos."));
}

void MainWindow::setSelectMode()
{
    scene->setEditMode(AnimationScene::EditMode::ModeSelect);
}

void MainWindow::setRectangleMode()
{
    scene->setEditMode(AnimationScene::EditMode::ModeRectangle);
}

void MainWindow::setEllipseMode()
{
    scene->setEditMode(AnimationScene::EditMode::ModeEllipse);
}

void MainWindow::selectionChanged(const QItemSelection& current,const QItemSelection&)
{
    scene->deselectAll();

    if(current.count() && current.at(0).indexes().count())
    {
        const QModelIndex index = current.at(0).indexes().at(0);
        QVariant v = index.data(Qt::UserRole);
        QGraphicsItem *item = (QGraphicsItem *) v.value<void *>();
        if(item)
            item->setSelected(true);
    }
    //qDebug() << current.at(0).indexes().at(0).row();

    // todo: set property page
}

void MainWindow::deleteItem()
{
    foreach(QGraphicsItem *item, scene->selectedItems())
    {
        scene->removeItem(item);
    }
    // todo: tree view
}

void MainWindow::sceneSeletionChanged()
{
    qDebug() << "scenesel";
    const QModelIndex index;

    //tree->setCurrentIndex(index);
}

void MainWindow::sceneItemAdded(QGraphicsItem *item)
{
    model->addItem(item);
    tree->reset();
    tree->expandAll();
}
