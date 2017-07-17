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
#include "animationscene.h"
#include "vectorgraphic.h"
#include "bitmap.h"
#include "exception.h"
#include "news.h"
#include "itempropertyeditor.h"
#include "timeline.h"
#include "scenepropertyeditor.h"
#include "transitioneditor.h"
#include "expander.h"
#include "rectangle.h"
#include "ellipse.h"
#include "text.h"
#include "bitmap.h"
#include "vectorgraphic.h"
#include "keyframe.h"
#include <QtTest/QTest>
#include <QMessageBox>
#include <QGraphicsSvgItem>
#include <QTreeWidget>
#include <QMainWindow>
#include <QtWidgets>

#define MAGIC 0x414D4200
#define FILE_VERSION 100

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    undoStack = new QUndoStack(this);

    setDockNestingEnabled(true);
    createStatusBar();
    createActions();
    createMenus();
    createGui();
    readSettings();
}

MainWindow::~MainWindow()
{
    delete scene;
    delete timeline;
    delete elementTree;
    delete view;
}

void MainWindow::save()
{
    writeFile(loadedFile.filePath());
}

void MainWindow::saveAs()
{
    QString fileName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setNameFilter(tr("AnimationMaker (*.amb);;All Files (*)"));
    dialog->setWindowTitle(tr("Save Animation"));
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    if(dialog->exec())
        fileName = dialog->selectedFiles().first();
    delete dialog;
    if(fileName.isEmpty())
        return;

    writeFile(fileName);
    loadedFile.setFile(fileName);
    saveAct->setEnabled(true);
    setTitle();
}

void MainWindow::writeFile(QString fileName)
{
    scene->clearSelection();
    timeline->setPlayheadPosition(0);

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Error", "Unable to open file " + fileName);
        return;
    }
    QDataStream out(&file);

    // Write a header with a "magic number" and a version
    out << (quint32)MAGIC;
    out << (qint32)FILE_VERSION;
    out.setVersion(QDataStream::Qt_5_7);

    out << scene;

    file.close();

    undoStack->clear();
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
    timeline->reset();
}

void MainWindow::newfile()
{
    reset();
    fillTree();
    saveAct->setEnabled(false);
    loadedFile.setFile("");
    setTitle();
    m_scenePropertyEditor->setScene(scene);
    propertiesdock->setWidget(m_scenePropertyEditor);
}

void MainWindow::open()
{
    QString fileName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setNameFilter(tr("AnimationMaker (*.amb);;All Files (*)"));
    dialog->setWindowTitle(tr("Open Animation"));
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    if(dialog->exec())
        fileName = dialog->selectedFiles().first();
    delete dialog;
    if (fileName.isEmpty())
        return;
    reset();
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Error", "Unable to open file " + fileName);
        return;
    }
    QDataStream in(&file);

    // Read and check the header
    quint32 magic;
    in >> magic;
    if (magic != MAGIC)
    {
        file.close();
        QMessageBox::warning(this, "AnimationMaker", "This file is not a valid AnimationMaker file!");
        return;
    }

    // Read the version
    qint32 version;
    in >> version;
    if (version < FILE_VERSION)
    {
        file.close();
        QMessageBox::warning(this, "AnimationMaker", "This file is not a valid AnimationMaker file!");
        return;
    }
    if (version > FILE_VERSION)
    {
        file.close();
        QMessageBox::warning(this, "AnimationMaker", "This file is not a valid AnimationMaker file!");
        return;
    }

    //    if (version <= 110)
    //        in.setVersion(QDataStream::Qt_4_0);
    //    else
    //        in.setVersion(QDataStream::Qt_5_7);

    // Read the data
    in >> scene;
    file.close();

    fillTree();
    elementTree->expandAll();
    m_scenePropertyEditor->setScene(scene);
    timeline->expandTree();
    loadedFile.setFile(fileName);
    saveAct->setEnabled(true);
    setTitle();
}

void MainWindow::fillTree()
{
    for(int i=root->childCount() - 1; i >= 0; i--)
    {
        QTreeWidgetItem *treeItem= root->child(i);
        root->removeChild(treeItem);
        delete treeItem;
    }

    QList<QGraphicsItem*> itemList = scene->items(Qt::AscendingOrder);
    foreach (QGraphicsItem *item, itemList)
    {
        ResizeableItem *ri = dynamic_cast<ResizeableItem*>(item);
        if(ri)
        {
            QTreeWidgetItem *treeItem = new QTreeWidgetItem();
            treeItem->setText(0, ri->id());
            treeItem->setIcon(0, QIcon(":/images/rect.png"));
            treeItem->setData(0, 3, qVariantFromValue((void *) ri));
            root->addChild(treeItem);
            connect(ri, SIGNAL(idChanged(ResizeableItem *, QString)), this, SLOT(idChanged(ResizeableItem *, QString)));
        }
    }
}

void MainWindow::idChanged(ResizeableItem *item, QString id)
{
    for(int i=0; i < root->childCount(); i++)
    {
        if(root->child(i)->data(0, 3).value<void *>() == item)
        {
            root->child(i)->setText(0, id);
            break;
        }
    }
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

    News *news = new News("https://artanidos.github.io/AnimationMaker/news.xml");
    newsdock = new QDockWidget(tr("News"), this);
    newsdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    newsdock->setWidget(news);
    newsdock->setObjectName("News");
    addDockWidget(Qt::RightDockWidgetArea, newsdock);

    tooldock = new QDockWidget(tr("Tools"), this);
    tooldock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tooldock->setWidget(toolpanel);
    tooldock->setObjectName("Tools");
    addDockWidget(Qt::LeftDockWidgetArea, tooldock);

    scene = new AnimationScene();
    scene->registerUndoStack(undoStack);

    m_itemPropertyEditor = new ItemPropertyEditor();
    m_scenePropertyEditor = new ScenePropertyEditor();
    m_transitionEditor = new TransitionEditor();
    m_transitionEditor->setUndoStack(undoStack);

    m_scenePropertyEditor->setScene(scene);

    propertiesdock = new QDockWidget(tr("Properties"), this);
    propertiesdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    propertiesdock->setWidget(m_scenePropertyEditor);
    propertiesdock->setObjectName("Properties");

    addDockWidget(Qt::RightDockWidgetArea, propertiesdock);

    view = new QGraphicsView(scene);
    view->setSceneRect(-100, -100, scene->width() + 200, scene->height() + 200);
    view->setRenderHint(QPainter::RenderHint::Antialiasing);
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(sceneSelectionChanged()));
    connect(scene, SIGNAL(itemAdded(QGraphicsItem*)), this, SLOT(sceneItemAdded(QGraphicsItem*)));
    connect(scene, SIGNAL(sizeChanged(int,int)), this, SLOT(sceneSizeChanged(int, int)));
    connect(scene, SIGNAL(itemRemoved(ResizeableItem*)), this, SLOT(sceneItemRemoved(ResizeableItem*)));
    connect(scene, SIGNAL(animationResetted()), this, SLOT(reset()));

    elementTree = new QTreeWidget();
    elementTree->header()->close();
    elementTree->setSelectionMode(QAbstractItemView::SingleSelection);
    root = new QTreeWidgetItem();
    root->setText(0, "Scene");
    elementTree->addTopLevelItem(root);
    connect(elementTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(elementTreeItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));

    elementsdock = new QDockWidget(tr("Elements"), this);
    elementsdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    elementsdock->setWidget(elementTree);
    elementsdock->setObjectName("Elements");
    addDockWidget(Qt::LeftDockWidgetArea, elementsdock);
    splitDockWidget(tooldock, elementsdock, Qt::Horizontal);

    timeline = new Timeline(scene);
    timeline->setMinimumHeight(110);

    connect(timeline, SIGNAL(itemSelectionChanged(ResizeableItem *)), this, SLOT(timelineSelectionChanged(ResizeableItem*)));
    connect(timeline, SIGNAL(transitionSelectionChanged(KeyFrame*)), this, SLOT(transitionSelectionChanged(KeyFrame*)));
    connect(m_itemPropertyEditor, SIGNAL(addKeyFrame(ResizeableItem*,QString,QVariant)), timeline, SLOT(addKeyFrame(ResizeableItem*,QString,QVariant)));

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(view);
    splitter->addWidget(timeline);
    setCentralWidget(splitter);
}

void MainWindow::elementTreeItemChanged(QTreeWidgetItem *newItem, QTreeWidgetItem *)
{
    scene->clearSelection();
    ResizeableItem *item = (ResizeableItem *)  newItem->data(0, 3).value<void *>();
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

void MainWindow::sceneSizeChanged(int width, int height)
{
    view->setSceneRect(-100, -100, width + 200, height + 200);
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

    exportXmlAct = new QAction(tr("Export XML"), this);
    connect(exportXmlAct, SIGNAL(triggered()), this, SLOT(exportXml()));

    exportMovieAct = new QAction(tr("Export Movie"), this);
    connect(exportMovieAct, SIGNAL(triggered()), this, SLOT(exportMovie()));

    importXmlAct = new QAction(tr("Import XML"), this);
    connect(importXmlAct, SIGNAL(triggered()), this, SLOT(importXml()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    undoAct = undoStack->createUndoAction(this, tr("&Undo"));
    undoAct->setShortcuts(QKeySequence::Undo);

    redoAct = undoStack->createRedoAction(this, tr("&Redo"));
    redoAct->setShortcuts(QKeySequence::Redo);

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    delAct = new QAction(tr("&Delete"), this);
    delAct->setShortcut(QKeySequence::Delete);
    connect(delAct, SIGNAL(triggered()), this, SLOT(del()));

    showElementsAct = new QAction("Elements");
    connect(showElementsAct, SIGNAL(triggered()), this, SLOT(showElementsPanel()));

    showPropertyPanelAct = new QAction("Properties");
    connect(showPropertyPanelAct, SIGNAL(triggered()), this, SLOT(showPropertyPanel()));

    showToolPanelAct = new QAction("Tools");
    connect(showToolPanelAct, SIGNAL(triggered()), this, SLOT(showToolPanel()));

    showNewsPanelAct = new QAction("News");
    connect(showNewsPanelAct, SIGNAL(triggered()), this, SLOT(showNewsPanel()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    addAction(copyAct);
    addAction(pasteAct);
    addAction(exitAct);
    addAction(delAct);
}

void MainWindow::createMenus()
{
    importMenu = new QMenu(tr("Import"));
    importMenu->setEnabled(false);

    exportMenu = new QMenu(tr("Export"));
    exportMenu->setEnabled(false);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(importXmlAct);
    fileMenu->addAction(exportXmlAct);
    fileMenu->addAction(exportMovieAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(delAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(showToolPanelAct);
    viewMenu->addAction(showElementsAct);
    viewMenu->addAction(showPropertyPanelAct);
    viewMenu->addAction(showNewsPanelAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

    this->addAction(undoAct);
    this->addAction(redoAct);
    this->addAction(copyAct);
    this->addAction(pasteAct);
}

void MainWindow::about()
{
    QMessageBox msg;
    msg.setWindowTitle("About AnimationMaker (Community Edition)");
    msg.setText("AnimationMaker\nVersion: " + QCoreApplication::applicationVersion() + "\n(C) Copyright 2017 Olaf Japp. All rights reserved.\n\nThe program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n\nTHIS COMMUNITY EDITION IS FOR NON COMMERCIAL USAGE ONLY!");
    msg.setIconPixmap(QPixmap(":/images/logo.png"));
    msg.exec();
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

void MainWindow::sceneSelectionChanged()
{
    ResizeableItem *item = NULL;

    if(scene->selectedItems().count())
        item = dynamic_cast<ResizeableItem*>(scene->selectedItems().first());
    if(item)
    {
        m_itemPropertyEditor->setItem(item);
        propertiesdock->setWidget(m_itemPropertyEditor);
        for(int i=0; i<root->childCount(); i++)
        {
            QTreeWidgetItem *treeItem = root->child(i);
            if(treeItem->data(0, 3).value<void *>() == item)
                treeItem->setSelected(true);
            else
                treeItem->setSelected(false);
        }
        root->setSelected(false);

        timeline->selectItem(item);
    }
    else
    {
        propertiesdock->setWidget(m_scenePropertyEditor);
        for(int i=0; i<root->childCount(); i++)
            root->child(i)->setSelected(false);
        root->setSelected(true);
        timeline->selectItem(NULL);
    }
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
    ResizeableItem *ri = dynamic_cast<ResizeableItem*>(item);
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, ri->id());
    treeItem->setIcon(0, QIcon(":/images/rect.png"));
    treeItem->setData(0, 3, qVariantFromValue((void *) ri));
    connect(ri, SIGNAL(idChanged(ResizeableItem *, QString)), this, SLOT(idChanged(ResizeableItem *, QString)));

    root->addChild(treeItem);
    root->setExpanded(true);
    root->setSelected(false);
    for(int i=0; i<root->childCount(); i++)
        root->child(i)->setSelected(false);
    treeItem->setSelected(true);

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

void MainWindow::showElementsPanel()
{
    elementsdock->setVisible(true);
}

void MainWindow::showNewsPanel()
{
    newsdock->setVisible(true);
}

void MainWindow::copy()
{
    scene->copyItem();
}

void MainWindow::paste()
{
    scene->pasteItem();
}

void MainWindow::del()
{
    while(scene->selectedItems().count())
    {
        ResizeableItem *item = dynamic_cast<ResizeableItem*>(scene->selectedItems().first());
        if(item)
            scene->deleteItem(item);
    }
}

void MainWindow::sceneItemRemoved(ResizeableItem *item)
{
    for(int i=0; i<root->childCount(); i++)
    {
        if(root->child(i)->data(0, 3).value<void *>() == item)
        {
            QTreeWidgetItem *treeItem = root->child(i);
            root->removeChild(treeItem);
            delete treeItem;
            break;
        }
    }
    timeline->removeItem(item);
}

void MainWindow::transitionSelectionChanged(KeyFrame *frame)
{
    if(frame)
    {
        scene->clearSelection();
        m_transitionEditor->setKeyframe(frame);
        propertiesdock->setWidget(m_transitionEditor);
    }
    else
        propertiesdock->setWidget(m_scenePropertyEditor);
}

void MainWindow::importXml()
{
    QString fileName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setNameFilter("XML format (*.xml);;All Files (*)");
    dialog->setWindowTitle("Import Animation from XML");
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    if(dialog->exec())
        fileName = dialog->selectedFiles().first();
    delete dialog;
    if(fileName.isEmpty())
        return;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0, "Error", "Unable to open file " + fileName);
        statusBar()->showMessage("Unable to open file " + fileName);
        return;
    }
    statusBar()->showMessage("Reading from file " + fileName);
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        QMessageBox::warning(0, "Error", "Unable to read file " + fileName);
        file.close();
        statusBar()->showMessage("Unable to read file " + fileName);
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    if(docElem.nodeName() == "Animation")
    {
        scene->reset();
        scene->setFps(docElem.attribute("fps", "24").toInt());
        scene->setWidth(docElem.attribute("width", "1200").toInt());
        scene->setHeight(docElem.attribute("height", "720").toInt());
    }
    for(int i=0; i < docElem.childNodes().count(); i++)
    {
        QDomNode node =docElem.childNodes().at(i);
        if(node.nodeName() == "Rectangle")
        {
            QDomElement ele = node.toElement();
            Rectangle *r = new Rectangle(ele.attribute("width", "50").toDouble(), ele.attribute("height", "50").toDouble(), scene);
            r->setId(ele.attribute("id", "Rectangle"));
            r->setLeft(ele.attribute("left", "0").toDouble());
            r->setTop(ele.attribute("top", "0").toDouble());
            r->setPen(QPen(QColor(ele.attribute("pen", "#000000"))));
            r->setBrush(QBrush(QColor(ele.attribute("brush", "#0000FF"))));
            r->setOpacity(ele.attribute("opacity", "100").toInt());
            r->setFlag(QGraphicsItem::ItemIsMovable, true);
            r->setFlag(QGraphicsItem::ItemIsSelectable, true);
            readKeyframes(&ele, r);
            scene->addItem(r);
        }
        else if(node.nodeName() == "Ellipse")
        {
            QDomElement ele = node.toElement();
            Ellipse *e = new Ellipse(ele.attribute("width", "50").toDouble(), ele.attribute("height", "50").toDouble(), scene);
            e->setId(ele.attribute("id", "Ellipse"));
            e->setHeight(ele.attribute("height", "50").toDouble());
            e->setLeft(ele.attribute("left", "0").toDouble());
            e->setTop(ele.attribute("top", "0").toDouble());
            e->setPen(QPen(QColor(ele.attribute("pen", "#000000"))));
            e->setBrush(QBrush(QColor(ele.attribute("brush", "#0000FF"))));
            e->setOpacity(ele.attribute("opacity", "100").toInt());
            e->setFlag(QGraphicsItem::ItemIsMovable, true);
            e->setFlag(QGraphicsItem::ItemIsSelectable, true);
            readKeyframes(&ele, e);
            scene->addItem(e);
        }
        else if(node.nodeName() == "Text")
        {
            QDomElement ele = node.toElement();
            Text *t = new Text(ele.attribute("text"), scene);
            t->setId(ele.attribute("id", "Text"));
            t->setLeft(ele.attribute("left", "0").toDouble());
            t->setTop(ele.attribute("top", "0").toDouble());
            t->setScale(ele.attribute("xscale", "1").toDouble(), ele.attribute("yscale", "1").toDouble());
            t->setTextcolor(QColor(ele.attribute("textcolor", "#000000")));
            t->setOpacity(ele.attribute("opacity", "100").toInt());
            QFont font;
            font.setFamily(ele.attribute("font-family"));
            font.setPointSize(ele.attribute("font-size").toInt());
            font.setStyleName(ele.attribute("font-style"));
            t->setFont(font);
            t->setFlag(QGraphicsItem::ItemIsMovable, true);
            t->setFlag(QGraphicsItem::ItemIsSelectable, true);
            readKeyframes(&ele, t);
            scene->addItem(t);
        }
        else if(node.nodeName() == "Bitmap")
        {
            QDomElement ele = node.toElement();
            QDomNode data = ele.firstChild();
            QDomCDATASection cdata = data.toCDATASection();
            QImage img = QImage::fromData(QByteArray::fromBase64(cdata.data().toLatin1()), "PNG");
            Bitmap *b = new Bitmap(img, ele.attribute("width", "50").toDouble(), ele.attribute("height", "50").toDouble(), scene);
            b->setId(ele.attribute("id", "Bitmap"));
            b->setLeft(ele.attribute("left", "0").toDouble());
            b->setTop(ele.attribute("top", "0").toDouble());
            b->setOpacity(ele.attribute("opacity", "100").toInt());
            b->setFlag(QGraphicsItem::ItemIsMovable, true);
            b->setFlag(QGraphicsItem::ItemIsSelectable, true);
            readKeyframes(&ele, b);
            scene->addItem(b);
        }
        else if(node.nodeName() == "Vectorgraphic")
        {
            QDomElement ele = node.toElement();
            QDomNode data = ele.firstChild();
            QDomCDATASection cdata = data.toCDATASection();
            Vectorgraphic *v = new Vectorgraphic(cdata.data().toLatin1(), scene);
            v->setId(ele.attribute("id", "Vectorgraphic"));
            v->setLeft(ele.attribute("left", "0").toDouble());
            v->setTop(ele.attribute("top", "0").toDouble());
            v->setScale(ele.attribute("xscale", "1").toDouble(), ele.attribute("yscale", "1").toDouble());
            v->setOpacity(ele.attribute("opacity", "100").toInt());
            v->setFlag(QGraphicsItem::ItemIsMovable, true);
            v->setFlag(QGraphicsItem::ItemIsSelectable, true);
            readKeyframes(&ele, v);
            scene->addItem(v);
        }
    }

    fillTree();
    elementTree->expandAll();
    m_scenePropertyEditor->setScene(scene);
    timeline->expandTree();
    statusBar()->showMessage(QString("Ready"));
}

void MainWindow::exportMovie()
{
    QString fileName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setNameFilter("Video format (*.mpg *.mp4 *.avi *.gif);;All Files (*)");
    dialog->setWindowTitle("Export Animation to Movie");
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    if(dialog->exec())
        fileName = dialog->selectedFiles().first();
    delete dialog;
    if(fileName.isEmpty())
        return;

    scene->clearSelection();
    view->setUpdatesEnabled(false);
    QGraphicsView *exportView = new QGraphicsView(scene);
    exportView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    exportView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    exportView->setGeometry(0,0,scene->width(), scene->height());

    int delay = 1000 / scene->fps();
    int frames = timeline->lastKeyframe() / delay + 2;

    QDir tmp = QDir::temp();
    tmp.mkdir("animationmaker");
    QString filterString = tmp.absolutePath() + "/animationmaker/frame%03d.png";
    tmp.cd("animationmaker");

    for (int i = 0; i < frames; i++)
    {
        statusBar()->showMessage(QString("Writing frame %1 of %2 frames").arg(i).arg(frames));

        timeline->setPlayheadPosition(i * delay);
        QTest::qSleep(delay);
        QCoreApplication::processEvents(QEventLoop::AllEvents, delay);

        QImage img = exportView->grab().toImage();
        QString imgName = QString::asprintf(filterString.toLatin1(), i);
        img.save(imgName);
    }

    statusBar()->showMessage(QString("Creating movie file"));
    QProcess *proc = new QProcess();
    proc->setWorkingDirectory("/home/olaf/SourceCode/PyTest");
    proc->start("python -c \"from export import *; print exportMovie('" + fileName + "', '" + tmp.absolutePath() + "', " + QString::number(scene->fps()) + ")\"");
    proc->waitForFinished();
    qDebug() << proc->readAllStandardOutput();
    qDebug() << proc->readAllStandardError();
    delete proc;

    tmp.removeRecursively();
    view->setUpdatesEnabled(true);
    statusBar()->showMessage("Ready");
    delete exportView;
}

void MainWindow::exportXml()
{
    QString fileName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setNameFilter("XML format (*.xml);;All Files (*)");
    dialog->setWindowTitle("Export Animation to XML");
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    if(dialog->exec())
        fileName = dialog->selectedFiles().first();
    delete dialog;
    if(fileName.isEmpty())
        return;

    bool exportAll = scene->selectedItems().count() == 0;

    QDomDocument doc;
    QDomElement root;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(0, "Error", "Unable to open file " + fileName);
        statusBar()->showMessage("Unable to open file " + fileName);
        return;
    }
    statusBar()->showMessage("Writing to file " + fileName);

    if(exportAll)
    {
        root = doc.createElement("Animation");
        root.setAttribute("fps", scene->fps());
        root.setAttribute("width", scene->width());
        root.setAttribute("height", scene->height());
        doc.appendChild(root);
    }
    else
    {
        root = doc.createElement("AnimationItems");
        doc.appendChild(root);
    }

    for(int i=0; i < scene->items().count(); i++)
    {
        QGraphicsItem *item = scene->items().at(i);
        Rectangle *r = dynamic_cast<Rectangle*>(item);
        if(r)
        {
            QDomElement rect = doc.createElement("Rectangle");
            rect.setAttribute("id", r->id());
            rect.setAttribute("left", QVariant(r->left()).toString());
            rect.setAttribute("top", QVariant(r->top()).toString());
            rect.setAttribute("width", QVariant(r->rect().width()).toString());
            rect.setAttribute("height", QVariant(r->rect().height()).toString());
            rect.setAttribute("pen", r->pen().color().name());
            rect.setAttribute("brush", r->brush().color().name());
            rect.setAttribute("opacity", r->opacity());
            writeKeyframes(&doc, &rect, r);
            root.appendChild(rect);
        }
        Ellipse *e = dynamic_cast<Ellipse*>(item);
        if(e)
        {
            QDomElement ellipse = doc.createElement("Ellipse");
            ellipse.setAttribute("id", e->id());
            ellipse.setAttribute("left", QVariant(e->left()).toString());
            ellipse.setAttribute("top", QVariant(e->top()).toString());
            ellipse.setAttribute("width", QVariant(e->rect().width()).toString());
            ellipse.setAttribute("height", QVariant(e->rect().height()).toString());
            ellipse.setAttribute("pen", e->pen().color().name());
            ellipse.setAttribute("brush", e->brush().color().name());
            ellipse.setAttribute("opacity", e->opacity());
            writeKeyframes(&doc, &ellipse, e);
            root.appendChild(ellipse);
        }
        Text *t = dynamic_cast<Text*>(item);
        if(t)
        {
            QDomElement text = doc.createElement("Text");
            text.setAttribute("id", t->id());
            text.setAttribute("left", QVariant(t->left()).toString());
            text.setAttribute("top", QVariant(t->top()).toString());
            text.setAttribute("xscale", QVariant(t->xscale()).toString());
            text.setAttribute("yscale", QVariant(t->yscale()).toString());
            text.setAttribute("text", t->text());
            text.setAttribute("textcolor", t->textcolor().name());
            text.setAttribute("opacity", t->opacity());
            text.setAttribute("font-family", t->font().family());
            text.setAttribute("font-size", t->font().pointSize());
            text.setAttribute("font-style", t->font().styleName());
            writeKeyframes(&doc, &text, t);
            root.appendChild(text);
        }
        Bitmap *b = dynamic_cast<Bitmap*>(item);
        if(b)
        {
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            b->getImage().save(&buffer, "PNG");
            QDomElement bitmap = doc.createElement("Bitmap");
            bitmap.setAttribute("id", b->id());
            bitmap.setAttribute("left", QVariant(b->left()).toString());
            bitmap.setAttribute("top", QVariant(b->top()).toString());
            bitmap.setAttribute("width", QVariant(b->rect().width()).toString());
            bitmap.setAttribute("height", QVariant(b->rect().height()).toString());
            bitmap.setAttribute("opacity", b->opacity());
            bitmap.appendChild(doc.createCDATASection(QString::fromLatin1(byteArray.toBase64().data())));
            writeKeyframes(&doc, &bitmap, b);
            root.appendChild(bitmap);
        }
        Vectorgraphic *v = dynamic_cast<Vectorgraphic*>(item);
        if(v)
        {
            QDomElement vectorgraphic = doc.createElement("Vectorgraphic");
            vectorgraphic.setAttribute("id", v->id());
            vectorgraphic.setAttribute("left", QVariant(v->left()).toString());
            vectorgraphic.setAttribute("top", QVariant(v->top()).toString());
            vectorgraphic.setAttribute("xscale", QVariant(v->xscale()).toString());
            vectorgraphic.setAttribute("yscale", QVariant(v->yscale()).toString());
            vectorgraphic.setAttribute("opacity", v->opacity());
            vectorgraphic.appendChild(doc.createCDATASection(QString::fromLatin1(v->getData())));
            writeKeyframes(&doc, &vectorgraphic, v);
            root.appendChild(vectorgraphic);
        }
    }
    QTextStream stream(&file);
    stream << doc.toString();
    file.close();
    statusBar()->showMessage(QString("Ready"));
}

void MainWindow::writeKeyframes(QDomDocument *doc, QDomElement *element, ResizeableItem *item)
{
    QHash<QString, KeyFrame*>::iterator it;
    for(it = item->keyframes()->begin(); it != item->keyframes()->end(); it++)
    {
        QDomElement frames = doc->createElement("Keyframes");
        frames.setAttribute("property", it.key());
        for(KeyFrame *frame = it.value(); frame != NULL; frame = frame->next())
        {
            QDomElement f = doc->createElement("Keyframe");
            f.setAttribute("time", frame->time());
            f.setAttribute("value", frame->value().toString());
            f.setAttribute("easing", frame->easing());
            frames.appendChild(f);
        }
        element->appendChild(frames);
    }
}

void MainWindow::readKeyframes(QDomElement *element, ResizeableItem *item)
{
    KeyFrame *m_tempKeyFrame = NULL;
    for(int i=0; i < element->childNodes().count(); i++)
    {
        QDomNode node = element->childNodes().at(i);
        if(node.nodeName() == "Keyframes")
        {
            QDomElement keyframes = node.toElement();
            for(int j=0; j < node.childNodes().count(); j++)
            {
                QDomNode frameNode = node.childNodes().at(j);
                if(frameNode.nodeName() == "Keyframe")
                {
                    QDomElement keyframe = frameNode.toElement();
                    KeyFrame *key = new KeyFrame();
                    key->setTime(keyframe.attribute("time", "0").toInt());
                    key->setValue(keyframe.attribute("value"));
                    key->setEasing(keyframe.attribute("easing", "-1").toInt());
                    // set double linked list
                    if(m_tempKeyFrame)
                    {
                        m_tempKeyFrame->setNext(key);
                        key->setPrev(m_tempKeyFrame);
                    }
                    else
                    {
                        item->addKeyframe(keyframes.attribute("property"), key);
                        emit scene->keyframeAdded(item, keyframes.attribute("property"), key);
                    }
                    m_tempKeyFrame = key;
                }
            }
            m_tempKeyFrame = NULL;
        }
    }
}
