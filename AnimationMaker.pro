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
QT += testlib xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AnimationMaker
TEMPLATE = app
target.path += /bin
INSTALLS += target

SOURCES += main.cpp\
        mainwindow.cpp \
    animationscene.cpp \
    rectangle.cpp \
    itemhandle.cpp \
    ellipse.cpp \
    text.cpp \
    resizeableitem.cpp \
    bitmap.cpp \
    vectorgraphic.cpp \
    timeline.cpp \
    expander.cpp \
    itempropertyeditor.cpp \
    scenepropertyeditor.cpp \
    transitionpanel.cpp \
    playhead.cpp \
    colorpicker.cpp \
    keyframe.cpp \
    transitionline.cpp \
    colorrect.cpp \
    commands.cpp \
    exception.cpp \
    coloreditor.cpp \
    transitioneditor.cpp \
    encode.cpp \
    news.cpp

HEADERS  += mainwindow.h \
    animationscene.h \
    rectangle.h \
    itemhandle.h \
    ellipse.h \
    text.h \
    resizeableitem.h \
    bitmap.h \
    vectorgraphic.h \
    timeline.h \
    expander.h \
    itempropertyeditor.h \
    scenepropertyeditor.h \
    transitionpanel.h \
    playhead.h \
    colorpicker.h \
    keyframe.h \
    transitionline.h \
    colorrect.h \
    commands.h \
    exception.h \
    coloreditor.h \
    transitioneditor.h \
    news.h \
    encode.h

linux-g++ {
    LIBS += -L$$PWD/lib/
    INCLUDEPATH += $$PWD/../../ffmpeg-3.2.2
    LIBS += -lm
    LIBS += -ldl
}
win32 {
    LIBS += -L$$PWD/../ffmpeg-3.2.4-win64-dev/lib/
    INCLUDEPATH += "C:\SourceCode\ffmpeg-3.2.4-win64-dev\include"
}


LIBS += -lavutil
LIBS += -lavformat
LIBS += -lavcodec
LIBS += -lswresample
LIBS += -lswscale


RESOURCES += \
    images.qrc

DISTFILES += \
    todo.txt \
    Deploy/AppImage/default.desktop \
    Deploy/AppImage/create.sh \
    Deploy/AppImage/default.png \
    Deploy/snapcraft/snap/snapcraft.yaml \
    Deploy/snapcraft/snap/gui/animationmaker.desktop \
    Deploy/snapcraft/snap/gui/icon.png \
    changelog.txt \
    build.txt \
    Deploy/snapcraft/bin/xdg-open

FORMS +=
