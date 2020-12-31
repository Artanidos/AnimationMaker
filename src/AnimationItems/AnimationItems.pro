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

QT += core widgets xml svg

TARGET = AnimationItems
TEMPLATE = lib
CONFIG  += static

OBJECTS_DIR = objs/
MOC_DIR     = mocs/

SOURCES += \
    coloreditor.cpp \
    colorpicker.cpp \
    colorrect.cpp \
    commands.cpp \
    expander.cpp \
    flatbutton.cpp \
    itemhandle.cpp \
    itempropertyeditor.cpp \
    keyframehandle.cpp \
    playhead.cpp \
    plugins.cpp \
    ruler.cpp \
    scenepropertyeditor.cpp \
    sceneview.cpp \
    svgattributeeditor.cpp \
    timeline.cpp \
    transition.cpp \
    transitioneditor.cpp \
    transitionhandle.cpp \
    transitionhandleleft.cpp \
    transitionhandleright.cpp \
    transitionline.cpp \
    xmleditor.cpp \
    xmlhighlighter.cpp \
    animationitem.cpp \
    animationscene.cpp \
    bitmap.cpp \
    ellipse.cpp \
    installer.cpp \
    keyframe.cpp \
    rectangle.cpp \
    text.cpp \
    vectorgraphic.cpp \


HEADERS += \
    coloreditor.h \
    colorpicker.h \
    colorrect.h \
    commands.h \
    expander.h \
    flatbutton.h \
    itemhandle.h \
    itempropertyeditor.h \
    keyframehandle.h \
    playhead.h \
    plugins.h \
    propertyeditorinterface.h \
    ruler.h \
    scenepropertyeditor.h \
    sceneview.h \
    svgattributeeditor.h \
    timeline.h \
    transitioneditor.h \
    transition.h \
    transitionhandle.h \
    transitionhandleleft.h \
    transitionhandleright.h \
    transitionline.h \
    xmleditor.h \
    xmlhighlighter.h \
    animationitem.h \
    animationscene.h\
    bitmap.h \
    ellipse.h \
    installer.h \
    interfaces.h \
    keyframe.h \
    rectangle.h \
    text.h \
    vectorgraphic.h \

RESOURCES += ../images.qrc
