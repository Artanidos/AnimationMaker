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
#    Foobar is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

QT += core gui svg
QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AnimationMaker
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    encode.cpp \
    treemodel.cpp \
    treeitem.cpp \
    animationscene.cpp \
    rectangle.cpp \
    itemhandle.cpp \
    ellipse.cpp \
    text.cpp \
    resizeableitem.cpp \
    bitmap.cpp \
    vectorgraphic.cpp \
    timeline.cpp \
    timelinemodel.cpp \
    animationpropertyeditor.cpp \
    expander.cpp \
    itempropertyeditor.cpp \
    scenepropertyeditor.cpp \
    transitionpanel.cpp \
    playhead.cpp \
    colorpicker.cpp

HEADERS  += mainwindow.h \
    treemodel.h \
    treeitem.h \
    animationscene.h \
    rectangle.h \
    itemhandle.h \
    ellipse.h \
    text.h \
    resizeableitem.h \
    bitmap.h \
    vectorgraphic.h \
    timeline.h \
    timelinemodel.h \
    animationpropertyeditor.h \
    expander.h \
    itempropertyeditor.h \
    scenepropertyeditor.h \
    transitionpanel.h \
    playhead.h \
    colorpicker.h

INCLUDEPATH += $$PWD/../../ffmpeg-3.2.2

LIBS += -L$$PWD/lib/


LIBS += -lavutil
LIBS += -lavformat
LIBS += -lavcodec
LIBS += -lswresample
LIBS += -lavfilter
LIBS += -lswscale
LIBS += -lm
LIBS += -ldl

RESOURCES += \
    images.qrc

DISTFILES += \
    todo.txt

FORMS +=

