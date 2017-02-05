#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treemodel.h"
#include "animationscene.h"
#include "rectangle.h"
#include "ellipse.h"
#include "vectorgraphic.h"
#include "text.h"
#include "bitmap.h"

#include <QtTest/QTest>
#include <QMessageBox>
#include <QGraphicsSvgItem>

void video_encode(const char *filename, QGraphicsView *view, QParallelAnimationGroup *group);

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
    delete model;
}

void MainWindow::save()
{
    writeFile(loadedFile.filePath());
}

void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Animation"), "", tr("AnimationMaker (*.amb);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    writeFile(fileName);
    loadedFile.setFile(fileName);
    saveAct->setEnabled(true);
    setTitle();
}

void MainWindow::writeFile(QString fileName)
{
    scene->clearSelection();

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Error", "Unable to open file " + fileName);
        return;
    }
    QDataStream out(&file);

    // Write a header with a "magic number" and a version
    out << (quint32)0xA0B0C0D0;
    out << (qint32)123;
    out.setVersion(QDataStream::Qt_5_7);

    out << scene;

    file.close();
}

void MainWindow::setTitle()
{
    setWindowTitle(QCoreApplication::applicationName() + " - " + loadedFile.completeBaseName() + "." + loadedFile.suffix());
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
    setTitle();
}

void MainWindow::createGui()
{
    QToolBar *toolpanel = new QToolBar();
    toolpanel->setOrientation(Qt::Vertical);
    QActionGroup *anActionGroup = new QActionGroup(toolpanel);
    selectAct = new QAction("Select", anActionGroup);
    selectAct->setIcon(QIcon(":/images/arrow.png"));
    selectAct->setCheckable(true);

    rectangleAct = new QAction("Rectangle", anActionGroup);
    rectangleAct->setIcon(QIcon(":/images/rectangle.png"));
    rectangleAct->setCheckable(true);

    ellipseAct = new QAction("Ellipse", anActionGroup);
    ellipseAct->setIcon(QIcon(":/images/ellipse.png"));
    ellipseAct->setCheckable(true);

    textAct = new QAction("Text", anActionGroup);
    textAct->setIcon(QIcon(":/images/text.png"));
    textAct->setCheckable(true);

    bitmapAct = new QAction("Bitmap", anActionGroup);
    bitmapAct->setIcon(QIcon(":/images/camera.png"));
    bitmapAct->setCheckable(true);

    svgAct = new QAction("Vectorgraphic", anActionGroup);
    svgAct->setIcon(QIcon(":/images/svg.png"));
    svgAct->setCheckable(true);

    connect(selectAct, SIGNAL(triggered()), this, SLOT(setSelectMode()));
    connect(rectangleAct, SIGNAL(triggered()), this, SLOT(setRectangleMode()));
    connect(ellipseAct, SIGNAL(triggered()), this, SLOT(setEllipseMode()));
    connect(textAct, SIGNAL(triggered()), this, SLOT(setTextMode()));
    connect(bitmapAct, SIGNAL(triggered()), this, SLOT(setBitmapMode()));
    connect(svgAct, SIGNAL(triggered()), this, SLOT(setSvgMode()));

    toolpanel->addAction(selectAct);
    toolpanel->addAction(rectangleAct);
    toolpanel->addAction(ellipseAct);
    toolpanel->addAction(textAct);
    toolpanel->addAction(bitmapAct);
    toolpanel->addAction(svgAct);

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

    scene = new AnimationScene(itemMenu);
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
    Rectangle *rect = new Rectangle(100, 100, itemMenu);
    rect->setPen(QPen(Qt::black));
    rect->setBrush(QBrush(Qt::blue));
    rect->setPos(0, 0);
    rect->setOpacity(0);
    scene->addItem(rect);

    QPropertyAnimation *animation = new QPropertyAnimation(rect, "opacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);

    QPropertyAnimation *animationx = new QPropertyAnimation(rect, "x");
    animationx->setDuration(1000);
    animationx->setStartValue(0);
    animationx->setEndValue(400);
    animationx->setEasingCurve(QEasingCurve::InCubic);

    QPropertyAnimation *animationy = new QPropertyAnimation(rect, "y");
    animationy->setDuration(1000);
    animationy->setStartValue(0);
    animationy->setEndValue(50);

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(animation);
    group->addAnimation(animationx);
    group->addAnimation(animationy);
    group->start();
    group->pause();

    QGraphicsView *view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    view->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    view->setGeometry(0,0,scene->width(), scene->height());
    video_encode("test.mpg", view, group);
}

void MainWindow::playAnimation()
{
    QGraphicsSvgItem *item = new QGraphicsSvgItem("/home/olaf/Bilder/Lotus.svg");
    item->setScale(0.5);
    scene->addItem(item);
    Rectangle *rect = new Rectangle(100, 100, itemMenu);
    rect->setPen(QPen(Qt::black));
    rect->setBrush(QBrush(Qt::blue));
    rect->setPos(0, 0);
    rect->setOpacity(0);
    scene->addItem(rect);

    QPropertyAnimation *animation = new QPropertyAnimation(rect, "opacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);

    QPropertyAnimation *animationx = new QPropertyAnimation(rect, "x");
    animationx->setDuration(1000);
    animationx->setStartValue(0);
    animationx->setEndValue(400);
    animationx->setEasingCurve(QEasingCurve::InCubic);

    QPropertyAnimation *animationy = new QPropertyAnimation(rect, "y");
    animationy->setDuration(1000);
    animationy->setStartValue(0);
    animationy->setEndValue(50);

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(animation);
    group->addAnimation(animationx);
    group->addAnimation(animationy);

    group->start();
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

    bringToFrontAct = new QAction("Bring to front");
    connect(bringToFrontAct, SIGNAL(triggered()), this, SLOT(bringToFront()));

    sendToBackAct = new QAction("Send to back");
    connect(sendToBackAct, SIGNAL(triggered()), this, SLOT(sendToBack()));

    raiseAct = new QAction("Raise");
    connect(raiseAct, SIGNAL(triggered()), this, SLOT(raise()));

    lowerAct = new QAction("Lower");
    connect(lowerAct, SIGNAL(triggered()), this, SLOT(lower()));

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

    itemMenu = new QMenu();
    itemMenu->addAction(delAct);
    itemMenu->addSeparator();
    itemMenu->addAction(bringToFrontAct);
    itemMenu->addAction(raiseAct);
    itemMenu->addAction(lowerAct);
    itemMenu->addAction(sendToBackAct);

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

void MainWindow::setTextMode()
{
    scene->setEditMode(AnimationScene::EditMode::ModeText);
}

void MainWindow::setBitmapMode()
{
    scene->setEditMode(AnimationScene::EditMode::ModeBitmap);
}

void MainWindow::setSvgMode()
{
    scene->setEditMode(AnimationScene::EditMode::ModeSvg);
}

void MainWindow::selectionChanged(const QItemSelection& current,const QItemSelection&)
{
    scene->clearSelection();

    if(current.count() && current.at(0).indexes().count())
    {
        const QModelIndex index = current.at(0).indexes().at(0);
        QVariant v = index.data(Qt::UserRole);
        QGraphicsItem *item = (QGraphicsItem *) v.value<void *>();
        if(item)
            item->setSelected(true);
    }

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

static bool isAnimationMakerItem(QGraphicsItem *item)
{
    switch(item->type())
    {
        case Rectangle::Type:
        case Ellipse::Type:
        case Text::Type:
        case Bitmap::Type:
        case Vectorgraphic::Type:
            return true;
    }
    return false;
}

void MainWindow::lower()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    int pos = scene->items().indexOf(selectedItem);
    for(int i = pos + 1; i < scene->items().count(); i++)
    {
        QGraphicsItem *item = scene->items().at(i);
        if(isAnimationMakerItem(item))
        {
            selectedItem->stackBefore(item);
            break;
        }
    }
    // trick to repaint item
    selectedItem->setSelected(false);
    selectedItem->setSelected(true);
}

void MainWindow::raise()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    int pos = scene->items().indexOf(selectedItem);
    for(int i = pos - 1; i >= 0; i--)
    {
        QGraphicsItem *item = scene->items().at(i);
        if(isAnimationMakerItem(item))
        {
            item->stackBefore(selectedItem);
            break;
        }
    }
    // trick to repaint item
    selectedItem->setSelected(false);
    selectedItem->setSelected(true);
}

void MainWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    int pos = scene->items().indexOf(selectedItem);
    for(int i = pos - 1; i >= 0; i--)
    {
        QGraphicsItem *item = scene->items().at(i);
        if(isAnimationMakerItem(item))
        {
            item->stackBefore(selectedItem);
        }
    }
    // trick to repaint item
    selectedItem->setSelected(false);
    selectedItem->setSelected(true);
}

void MainWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    int pos = scene->items().indexOf(selectedItem);
    for(int i = pos + 1; i < scene->items().count(); i++)
    {
        QGraphicsItem *item = scene->items().at(i);
        if(isAnimationMakerItem(item))
        {
            selectedItem->stackBefore(item);
        }
    }
    // trick to repaint item
    selectedItem->setSelected(false);
    selectedItem->setSelected(true);
}

void MainWindow::sceneSeletionChanged()
{
    const QModelIndex index;

    //tree->setCurrentIndex(index);
}

void MainWindow::sceneItemAdded(QGraphicsItem *item)
{
    model->addItem(item);
    tree->reset();
    tree->expandAll();
}
