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

void video_encode(const char *filename, QGraphicsView *view, QParallelAnimationGroup *group, int fps, int length, MainWindow *win);

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
    delete timeline;
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
    if(loadedFile.completeBaseName().isEmpty())
        setWindowTitle(QCoreApplication::applicationName());
    else
        setWindowTitle(QCoreApplication::applicationName() + " - " + loadedFile.completeBaseName() + "." + loadedFile.suffix());
}

void MainWindow::reset()
{
    scene->reset();
    model->reset();
    timeline->reset();
}

void MainWindow::newfile()
{
    reset();
    saveAct->setEnabled(false);
    loadedFile.setFile("");
    setTitle();
    m_scenePropertyEditor->setScene(scene);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Animation"), "", tr("AnimationMaker (*.amb);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    reset();
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
    m_scenePropertyEditor->setScene(scene);
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

    tooldock = new QDockWidget(tr("Tools"), this);
    tooldock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tooldock->setWidget(toolpanel);
    tooldock->setObjectName("Tools");
    addDockWidget(Qt::LeftDockWidgetArea, tooldock);

    scene = new AnimationScene();
    scene->setSceneRect(QRect(0,0,1200,720));

    m_animationPropertyEditor = new AnimationPropertyEditor();
    m_itemPropertyEditor = new ItemPropertyEditor();
    m_scenePropertyEditor = new ScenePropertyEditor();

    m_scenePropertyEditor->setScene(scene);

    propertiesdock = new QDockWidget(tr("Properties"), this);
    propertiesdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    propertiesdock->setWidget(m_scenePropertyEditor);
    propertiesdock->setObjectName("Properties");

    addDockWidget(Qt::RightDockWidgetArea, propertiesdock);

    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::RenderHint::Antialiasing);
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(sceneSelectionChanged()));
    connect(scene, SIGNAL(itemAdded(QGraphicsItem*)), this, SLOT(sceneItemAdded(QGraphicsItem*)));

    model = new TreeModel();
    model->setScene(scene);
    tree = new QTreeView();
    tree->setModel(model);
    tree->header()->close();
    tree->expandAll();
    tree->setMinimumWidth(320);
    tree->setCurrentIndex(model->index(0, 0));
    QItemSelectionModel *selectionModel = tree->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(elementtreeSelectionChanged(const QItemSelection&,const QItemSelection&)));

    QDockWidget *elementsdock = new QDockWidget(tr("Elements"), this);
    elementsdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    elementsdock->setWidget(tree);
    elementsdock->setObjectName("Elements");
    addDockWidget(Qt::LeftDockWidgetArea, elementsdock);
    splitDockWidget(tooldock, elementsdock, Qt::Horizontal);

    timeline = new Timeline(scene);
    timeline->setMinimumHeight(110);

    connect(timeline, SIGNAL(animationSelectionChanged(QPropertyAnimation *)), this, SLOT(changePropertyEditor(QPropertyAnimation *)));
    connect(timeline, SIGNAL(itemSelectionChanged(ResizeableItem *)), this, SLOT(timelineSelectionChanged(ResizeableItem*)));
    connect(m_animationPropertyEditor, SIGNAL(dataChanged()), timeline, SLOT(animationChanged()));

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(view);
    splitter->addWidget(timeline);
    setCentralWidget(splitter);
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Animation"), "", tr("Video format (*.mpg *.mp4 *.avi);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QGraphicsView *view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    view->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    view->setGeometry(0,0,scene->width(), scene->height());
    video_encode(fileName.toLatin1(), view, timeline->getAnimations(), scene->fps(), scene->length(), this);
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    newAct = new QAction(tr("&New"), this);
    connect(newAct, SIGNAL(triggered()), this, SLOT(newfile()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setEnabled(false);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exportAct = new QAction(tr("&Export"), this);
    connect(exportAct, SIGNAL(triggered()), this, SLOT(exportAnimation()));

    //const QIcon exitIcon = QIcon::fromTheme("application-exit");
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    showPropertyPanelAct = new QAction("Properties");
    connect(showPropertyPanelAct, SIGNAL(triggered()), this, SLOT(showPropertyPanel()));

    showToolPanelAct = new QAction("Tools");
    connect(showToolPanelAct, SIGNAL(triggered()), this, SLOT(showToolPanel()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    addAction(copyAct);
    addAction(pasteAct);
    addAction(exitAct);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exportAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(showToolPanelAct);
    viewMenu->addAction(showPropertyPanelAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("AnimationMaker"),
            tr("The AnimationMaker is a tool to create presentation videos.\nVersion: ") + QCoreApplication::applicationVersion());
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

void MainWindow::elementtreeSelectionChanged(const QItemSelection& current,const QItemSelection&)
{
    scene->clearSelection();

    if(current.count() && current.at(0).indexes().count())
    {
        const QModelIndex index = current.at(0).indexes().at(0);
        QVariant v = index.data(Qt::UserRole);
        ResizeableItem *item = (ResizeableItem *) v.value<void *>();
        if(item)
        {
            item->setSelected(true);
            m_itemPropertyEditor->setItem(item);
            propertiesdock->setWidget(m_itemPropertyEditor);
        }
        else
        {
            propertiesdock->setWidget(m_scenePropertyEditor);
        }
    }
}

void MainWindow::sceneSelectionChanged()
{
    ResizeableItem *item = NULL;
    if(scene->selectedItems().count())
        item = dynamic_cast<ResizeableItem*>(scene->selectedItems().first());
    if(item)
    {
        m_itemPropertyEditor->setItem(item);
        propertiesdock->setWidget(m_itemPropertyEditor);
    }
    else
        propertiesdock->setWidget(m_scenePropertyEditor);
}

void MainWindow::timelineSelectionChanged(ResizeableItem* item)
{
    scene->clearSelection();
    item->setSelected(true);

    m_itemPropertyEditor->setItem(item);
    propertiesdock->setWidget(m_itemPropertyEditor);
}

void MainWindow::sceneItemAdded(QGraphicsItem *item)
{
    model->addItem(dynamic_cast<ResizeableItem*>(item));
    tree->reset();
    tree->expandAll();

    item->setSelected(true);
    selectAct->setChecked(true);
    scene->setEditMode(AnimationScene::EditMode::ModeSelect);
}

void MainWindow::showPropertyPanel()
{
    propertiesdock->setVisible(true);
}

void MainWindow::showToolPanel()
{
     tooldock->setVisible(true);
}

void MainWindow::changePropertyEditor(QPropertyAnimation *anim)
{
    m_animationPropertyEditor->setAnimation(anim);
    propertiesdock->setWidget(m_animationPropertyEditor);
}

void MainWindow::copy()
{
   scene->copyItem();
}

void MainWindow::paste()
{
    scene->pasteItem();
}
