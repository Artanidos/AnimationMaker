/****************************************************************************
**  Copyright (C) 2018 Olaf Japp and CrowdWare
**
**  This file is part of AnimationMaker.
**
**  Commercial License Usage
**  Licensees holding valid commercial CrowdWare licenses may use this file in
**  accordance with the commercial license agreement provided with the
**  Software or, alternatively, in accordance with the terms contained in
**  a written agreement between you and CrowdWare.
**
**  GNU General Public License Usage
**  Alternatively, this file may be used under the terms of the GNU
**  General Public License version 3
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
#include <QApplication>
#include <QStyleFactory>
#include <QTest>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("AnimationMaker");
    QCoreApplication::setApplicationVersion("1.8.1");
#ifdef LINUX
    QCoreApplication::setOrganizationName("Artanidos");
#else
    QCoreApplication::setOrganizationName("CrowdWare");
#endif

    QFont newFont("Sans Serif", 10);
    a.setFont(newFont);
    a.setStyle(QStyleFactory::create("Fusion"));
    QPalette p = a.palette();

    p.setColor(QPalette::Window, QColor(53,53,53));
    p.setColor(QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Base, QColor(64, 66, 68));
    p.setColor(QPalette::AlternateBase, QColor(53,53,53));
    p.setColor(QPalette::ToolTipBase, Qt::white);
    p.setColor(QPalette::ToolTipText, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Button, QColor(53,53,53));
    p.setColor(QPalette::ButtonText, Qt::white);
    p.setColor(QPalette::BrightText, Qt::red);

    p.setColor(QPalette::Highlight, QColor(255,127,42));
    p.setColor(QPalette::HighlightedText, Qt::black);

    p.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
    a.setPalette(p);
    a.setWindowIcon(QIcon(QLatin1String(":/images/logo.svg")));

    MainWindow w;
    w.show();

    return a.exec();
}
