#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "treemodel.h"
#include "animationscene.h"

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
    QLabel *timeline;
    QToolBar *toolbar;
    QTreeView *tree;
    TreeModel *model;
    AnimationScene *scene;
    QGraphicsView *view;
    QFileInfo loadedFile;

    QAction *openAct;
    QAction *saveAct;
    QAction *playAct;
    QAction *saveAsAct;
    QAction *exportAct;
    QAction *delAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *selectAct;
    QAction *rectangleAct;
    QAction *ellipseAct;
    QAction *textAct;
    QAction *svgAct;
    QAction *bringToFrontAct;
    QAction *sendToBackAct;
    QAction *bitmapAct;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *itemMenu;

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
    void deleteItem();
    void bringToFront();
    void sendToBack();
    void sceneSeletionChanged();
    void sceneItemAdded(QGraphicsItem *);
};

#endif // MAINWINDOW_H
