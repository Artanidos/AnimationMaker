#    Copyright (C) 2017 Olaf Japp
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

QT       += widgets testlib xml svg

TARGET = Widgets
TEMPLATE = lib
DEFINES += WIDGETS_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    flatbutton.cpp \
    itemhandle.cpp \
    keyframe.cpp \
    animationscene.cpp \
    commands.cpp \
    timeline.cpp \
    playhead.cpp \
    transitionline.cpp \
    ellipse.cpp \
    rectangle.cpp \
    text.cpp \
    vectorgraphic.cpp \
    bitmap.cpp \
    plugins.cpp \
    expander.cpp \
    animationitem.cpp \
    coloreditor.cpp \
    colorpicker.cpp \
    colorrect.cpp \
    xmleditor.cpp \
    xmlhighlighter.cpp \
    installer.cpp \
    transition.cpp \
    keyframehandle.cpp \
    transitionhandleright.cpp \
    transitionhandleleft.cpp \
    transitionhandle.cpp
    

HEADERS +=\
    widgets_global.h \
    flatbutton.h \
    itemhandle.h \
    keyframe.h \
    animationscene.h \
    commands.h \
    timeline.h \
    playhead.h \
    transitionline.h \
    ellipse.h \
    rectangle.h \
    text.h \
    vectorgraphic.h \
    bitmap.h \
    plugins.h \
    ../interfaces.h \
    expander.h \
    propertyeditorinterface.h \
    animationitem.h \
    coloreditor.h \
    colorpicker.h \
    colorrect.h \
    xmleditor.h \
    xmlhighlighter.h \
    installer.h \
    transition.h \
    keyframehandle.h \
    transitionhandleright.h \
    transitionhandleleft.h \
    transitionhandle.h
    
unix {
    target.path = /usr/lib
    INSTALLS += target
}
