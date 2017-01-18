#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtQuick>
#include <QtWidgets>
#include <treemodel.h>

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

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    void createMenus();
    void createActions();
    void createStatusBar();
    void createGui();
    void registerTypes();
    void writeSettings();
    void readSettings();

    QQuickView *view;
    QSplitter *splitter;
    QScrollArea *scroll;
    QWidget *container;
    QLabel *timeline;
    QToolBar *toolbar;
    QTreeView *tree;
    TreeModel *model;

    QAction *playAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *aboutAct;
    QMenu *fileMenu;
    QMenu *helpMenu;

public slots:
    void exportAnimation();
    void playAnimation();
    void about();
};

#endif // MAINWINDOW_H
