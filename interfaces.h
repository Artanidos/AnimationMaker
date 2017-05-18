#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QStatusBar>
#include <QGraphicsView>

class ExportInterface : public QObject
{
    Q_OBJECT
public:
    virtual QString displayName() const = 0;
    virtual QString filter() const = 0;
    virtual QString title() const = 0;
    virtual int exportFile(const char *filename, QGraphicsView *view, int length, QStatusBar *bar, int fps) = 0;
signals:
    void setPlayheadPos(int);
};

#define ExportInterface_iid "org.artanidos.AnimationMaker.ExportInterface"
Q_DECLARE_INTERFACE(ExportInterface, ExportInterface_iid)

#endif // INTERFACES_H
