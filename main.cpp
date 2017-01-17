#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("CrowdWare");
    QCoreApplication::setApplicationName("AnimationMaker");
    QCoreApplication::setApplicationVersion("1.0");

    a.setStyle(QStyleFactory::create("Fusion"));
    QPalette p = a.palette();

    p.setColor(QPalette::Window, QColor(53,53,53));
    p.setColor(QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Base, QColor(15,15,15));
    p.setColor(QPalette::AlternateBase, QColor(53,53,53));
    p.setColor(QPalette::ToolTipBase, Qt::white);
    p.setColor(QPalette::ToolTipText, Qt::white);
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Button, QColor(53,53,53));
    p.setColor(QPalette::ButtonText, Qt::white);
    p.setColor(QPalette::BrightText, Qt::red);

    p.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
    p.setColor(QPalette::HighlightedText, Qt::black);

    p.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
    a.setPalette(p);


    MainWindow w;
    w.show();

    return a.exec();
}
