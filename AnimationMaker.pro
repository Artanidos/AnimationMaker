#-------------------------------------------------
#
# Project created by QtCreator 2016-12-19T11:03:34
#
#-------------------------------------------------

QT += core gui
QT += qml quick testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AnimationMaker
TEMPLATE = app

QML_IMPORT_PATH += /home/olaf/SourceCode/AnimationMaker

SOURCES += main.cpp\
        mainwindow.cpp \
    encode.cpp \
    ellipse.cpp \
    treemodel.cpp \
    treeitem.cpp \
    rectangle.cpp \
    text.cpp \
    animationscene.cpp \
    serializeableitem.cpp

HEADERS  += mainwindow.h \
    ellipse.h \
    treemodel.h \
    treeitem.h \
    rectangle.h \
    text.h \
    animationscene.h \
    serializeableitem.h

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

DISTFILES += \
    demo.qml

RESOURCES += \
    images.qrc

