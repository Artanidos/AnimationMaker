#    Copyright (C) 2018 Olaf Japp
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

QT += widgets testlib xml svg

INCLUDEPATH += ../App/widgets
OBJECTS_DIR = objs/
SOURCES = \
    testgui.cpp \
    ../App/widgets/animationitem.cpp \
    ../App/widgets/animationscene.cpp \
    ../App/widgets/bitmap.cpp \
    ../App/widgets/coloreditor.cpp \
    ../App/widgets/colorpicker.cpp \
    ../App/widgets/colorrect.cpp \
    ../App/widgets/commands.cpp \
    ../App/widgets/ellipse.cpp \
    ../App/widgets/expander.cpp \
    ../App/widgets/flatbutton.cpp \
    ../App/widgets/installer.cpp \
    ../App/widgets/itemhandle.cpp \
    ../App/widgets/itempropertyeditor.cpp \
    ../App/widgets/keyframe.cpp \
    ../App/widgets/keyframehandle.cpp \
    ../App/widgets/mainwindow.cpp \
    ../App/widgets/playhead.cpp \
    ../App/widgets/plugins.cpp \
    ../App/widgets/rectangle.cpp \
    ../App/widgets/ruler.cpp \
    ../App/widgets/scenepropertyeditor.cpp \
    ../App/widgets/sceneview.cpp \
    ../App/widgets/svgattributeeditor.cpp \
    ../App/widgets/text.cpp \
    ../App/widgets/timeline.cpp \
    ../App/widgets/transition.cpp \
    ../App/widgets/transitioneditor.cpp \
    ../App/widgets/transitionhandle.cpp \
    ../App/widgets/transitionhandleleft.cpp \
    ../App/widgets/transitionhandleright.cpp \
    ../App/widgets/transitionline.cpp \
    ../App/widgets/vectorgraphic.cpp \
    ../App/widgets/xmleditor.cpp \
    ../App/widgets/xmlhighlighter.cpp \

HEADERS += \
    ../App/widgets/animationitem.h \
    ../App/widgets/animationscene.h \
    ../App/widgets/bitmap.h \
    ../App/widgets/coloreditor.h \
    ../App/widgets/colorpicker.h \
    ../App/widgets/colorrect.h \
    ../App/widgets/commands.h \
    ../App/widgets/ellipse.h \
    ../App/widgets/expander.h \
    ../App/widgets/flatbutton.h \
    ../App/widgets/installer.h \
    ../App/widgets/interfaces.h \
    ../App/widgets/itemhandle.h \
    ../App/widgets/itempropertyeditor.h \
    ../App/widgets/keyframe.h \
    ../App/widgets/keyframehandle.h \
    ../App/widgets/mainwindow.h \
    ../App/widgets/playhead.h \
    ../App/widgets/plugins.h \
    ../App/widgets/propertyeditorinterface.h \
    ../App/widgets/rectangle.h \
    ../App/widgets/ruler.h \
    ../App/widgets/scenepropertyeditor.h \
    ../App/widgets/sceneview.h \
    ../App/widgets/svgattributeeditor.h \
    ../App/widgets/text.h \
    ../App/widgets/timeline.h \
    ../App/widgets/transitioneditor.h \
    ../App/widgets/transition.h \
    ../App/widgets/transitionhandle.h \
    ../App/widgets/transitionhandleleft.h \
    ../App/widgets/transitionhandleright.h \
    ../App/widgets/transitionline.h \
    ../App/widgets/vectorgraphic.h \
    ../App/widgets/widgets_global.h \
    ../App/widgets/xmleditor.h \
    ../App/widgets/xmlhighlighter.h \

DEFINES += TEST

