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

QT           += core widgets xml
TEMPLATE      = lib
CONFIG       += plugin
TARGET        = $$qtLibraryTarget(Pie)
INSTALLS     += target

OBJECTS_DIR   = objs/
MOC_DIR       = mocs/

DEPENDPATH   += ../../AnimationItems/
INCLUDEPATH  += ../../AnimationItems/
DESTDIR = $$OUT_PWD/../../../output/opt/animationmaker/plugins
target.path = /opt/animationmaker/plugins

LIBS         += -L$$OUT_PWD/../../AnimationItems -lAnimationItems

SOURCES      += \
    pieitem.cpp \
    propertyeditor.cpp \
    commands.cpp \

HEADERS      += \
    pieitem.h \
    propertyeditor.h \
    commands.h

RESOURCES += \
    images.qrc
