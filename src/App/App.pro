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

QT += core gui svg
QT += testlib xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

debug {
    DEFINES += DEBUG
}

TARGET = AnimationMaker
TEMPLATE = app
target.path += /bin
INSTALLS += target

SOURCES += main.cpp \
    widgets/animationitem.cpp \
    widgets/animationscene.cpp \
    widgets/bitmap.cpp \
    widgets/coloreditor.cpp \
    widgets/colorpicker.cpp \
    widgets/colorrect.cpp \
    widgets/commands.cpp \
    widgets/ellipse.cpp \
    widgets/expander.cpp \
    widgets/flatbutton.cpp \
    widgets/installer.cpp \
    widgets/itemhandle.cpp \
    widgets/itempropertyeditor.cpp \
    widgets/keyframe.cpp \
    widgets/keyframehandle.cpp \
    widgets/mainwindow.cpp \
    widgets/playhead.cpp \
    widgets/plugins.cpp \
    widgets/rectangle.cpp \
    widgets/ruler.cpp \
    widgets/scenepropertyeditor.cpp \
    widgets/sceneview.cpp \
    widgets/svgattributeeditor.cpp \
    widgets/text.cpp \
    widgets/timeline.cpp \
    widgets/transition.cpp \
    widgets/transitioneditor.cpp \
    widgets/transitionhandle.cpp \
    widgets/transitionhandleleft.cpp \
    widgets/transitionhandleright.cpp \
    widgets/transitionline.cpp \
    widgets/vectorgraphic.cpp \
    widgets/xmleditor.cpp \
    widgets/xmlhighlighter.cpp \


HEADERS += \
    widgets/animationitem.h \
    widgets/animationscene.h \
    widgets/bitmap.h \
    widgets/coloreditor.h \
    widgets/colorpicker.h \
    widgets/colorrect.h \
    widgets/commands.h \
    widgets/ellipse.h \
    widgets/expander.h \
    widgets/flatbutton.h \
    widgets/installer.h \
    widgets/interfaces.h \
    widgets/itemhandle.h \
    widgets/itempropertyeditor.h \
    widgets/keyframe.h \
    widgets/keyframehandle.h \
    widgets/mainwindow.h \
    widgets/playhead.h \
    widgets/plugins.h \
    widgets/propertyeditorinterface.h \
    widgets/rectangle.h \
    widgets/ruler.h \
    widgets/scenepropertyeditor.h \
    widgets/sceneview.h \
    widgets/svgattributeeditor.h \
    widgets/text.h \
    widgets/timeline.h \
    widgets/transitioneditor.h \
    widgets/transition.h \
    widgets/transitionhandle.h \
    widgets/transitionhandleleft.h \
    widgets/transitionhandleright.h \
    widgets/transitionline.h \
    widgets/vectorgraphic.h \
    widgets/widgets_global.h \
    widgets/xmleditor.h \
    widgets/xmlhighlighter.h \

linux-g++ {
    LIBS += -lm
    LIBS += -ldl
}
