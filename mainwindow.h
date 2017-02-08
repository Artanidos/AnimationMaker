#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "treemodel.h"
#include "animationscene.h"
#include "timeline.h"
#include "animationpropertyeditor.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setTitle();
    
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    void createMenus();
    void createActions();
    void createStatusBar();
    void createGui();
    void writeSettings();
    void readSettings();
    void writeFile(QString fileName);

    QSplitter *splitter;
    //QLabel *timeline;
    QToolBar *toolbar;
    QTreeView *tree;
    TreeModel *model;
    AnimationScene *scene;
    QGraphicsView *view;
    QFileInfo loadedFile;
    Timeline *timeline;
    AnimationPropertyEditor *m_animationPropertyEditor;
    QDockWidget *propertiesdock;
    QLabel *propertiespanel;

    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exportAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *selectAct;
    QAction *rectangleAct;
    QAction *ellipseAct;
    QAction *textAct;
    QAction *svgAct;
    QAction *bitmapAct;
    QAction *showPropertyPanelAct;
    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *viewMenu;

public slots:
    void exportAnimation();
    void playAnimation();
    void about();
    void save();
    void saveAs();
    void open();
    void setSelectMode();
    void setRectangleMode();
    void setEllipseMode();
    void setTextMode();
    void setBitmapMode();
    void setSvgMode();
    void selectionChanged(const QItemSelection&,const QItemSelection&);
    void sceneItemAdded(QGraphicsItem *);
    void showPropertyPanel();
    void changePropertyEditor(QPropertyAnimation *);
    void itemSelectionChanged(ResizeableItem *item);
};

#endif // MAINWINDOW_H
