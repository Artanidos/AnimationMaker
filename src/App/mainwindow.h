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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QActionGroup>
#include <QCloseEvent>
#include <QDockWidget>
#include <QFileInfo>
#include <QGraphicsItem>
#include <QMainWindow>
#include <QMenu>
#include <QProcess>
#include <QSplitter>
#include <QString>
#include <QToolBar>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QUndoStack>
#include <QWidget>

#include "animationitem.h"
#include "animationscene.h"
#include "itempropertyeditor.h"
#include "sceneview.h"
#include "scenepropertyeditor.h"
#include "timeline.h"
#include "transitioneditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setTitle();
    
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void createMenus();
    void createActions();
    void createStatusBar();
    void createGui();
    void install();
    void loadPlugins();
    void writeSettings();
    void readSettings();
    void writeFile(QString fileName);
    void fillTree();
    void runExport(QStringList &args, QString path);
    void addCheckboxes(QTreeWidgetItem *treeItem, AnimationItem *item);
    void addNewImage(AnimationScene::EditMode mode);

    QProcess *m_proc;
    QString m_url;
    QString m_err;
    QSplitter *m_splitter;
    QToolBar *toolbar;
    QTreeWidget *m_elementTree;
    AnimationScene *m_scene;
    SceneView *m_view;
    QFileInfo loadedFile;
    Timeline *m_timeline;
    ItemPropertyEditor *m_itemPropertyEditor;
    ScenePropertyEditor *m_scenePropertyEditor;
    TransitionEditor *m_transitionEditor;
    QDockWidget *m_propertiesdock;
    QDockWidget *m_tooldock;
    QDockWidget *m_elementsdock;
    QTreeWidgetItem *m_root;
    QAction *openAct;
    QAction *newAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *saveItemAsAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *m_selectAct;
    QAction *m_rectangleAct;
    QAction *m_ellipseAct;
    QAction *m_textAct;
    QAction *m_svgAct;
    QAction *m_bitmapAct;
    QAction *showPropertyPanelAct;
    QAction *showToolPanelAct;
    QAction *showElementsAct;
    QAction *showRulerAct;
    QAction *exportMovieAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *delAct;
    QActionGroup *exportActionGroup;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *viewMenu;
    QMenu *importMenu;
    QMenu *exportMenu;
    QUndoStack *m_undoStack;
    const QString FFMPEG = "ffmpeg";
    const QString PLUGIN_DIR = "plugins";
    
public slots:
    void reset();
    void exportMovie();
    void pluginExport();
    void about();
    void save();
    void saveAs();
    void saveItemAs();
    void open();
    void newfile();
    void setSelectMode();
    void setRectangleMode();
    void setEllipseMode();
    void setTextMode();
    void loadNewBitmap();
    void loadNewSvg();
    void setPluginMode();
    void elementTreeItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);
    void sceneItemAdded(QGraphicsItem *);
    void showPropertyPanel();
    void showToolPanel();
    void showElementsPanel();
    void showRuler();
    void sceneSelectionChanged();
    void timelineSelectionChanged(AnimationItem*);
    void copy();
    void paste();
    void del();
    void sceneSizeChanged(int width, int height);
    void sceneItemRemoved(AnimationItem *item);
    void idChanged(AnimationItem *, QString);
    void transitionSelectionChanged(KeyFrame *frame);
    void changeZoom(int zoom);
    void elementVisibleStateChanged(int);
    void elementLockStateChanged(int);
};

#endif // MAINWINDOW_H
