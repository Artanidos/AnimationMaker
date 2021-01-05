#-------------------------------------------------
#
#    Project created by QtCreator 2016-12-19T11:03:34
#
#-------------------------------------------------
#    Copyright (C) 2016 Olaf Japp
#    https://facebook.com/artanidos
#
#    This file is part of AnimationMaker.
#
#    AnimationMaker is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    AnimationMaker is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.

QT += core gui widgets svg xml

debug {
    DEFINES += DEBUG
}

TARGET = AnimationMaker
TEMPLATE = app
INSTALLS += target

OBJECTS_DIR = objs/
MOC_DIR = mocs/
INCLUDEPATH += ../AnimationItems/
DESTDIR = $$OUT_PWD/../../output
target.path = /opt/animationmaker/

LIBS += -L$$OUT_PWD/../AnimationItems -lAnimationItems

SOURCES += main.cpp \
    mainwindow.cpp \


HEADERS += \
    mainwindow.h \

RESOURCES += ../images.qrc
