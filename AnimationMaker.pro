#-------------------------------------------------
#
# Project created by QtCreator 2016-12-19T11:03:34
#
#-------------------------------------------------

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
    itempropertyeditor.cpp

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
    itempropertyeditor.h

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

