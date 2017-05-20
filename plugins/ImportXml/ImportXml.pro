#-------------------------------------------------
#
# Project created by QtCreator 2017-05-20T10:39:18
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
#    along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.

QT += xml testlib gui widgets
TARGET = ImportXml
CONFIG += plugin
TEMPLATE = lib
INCLUDEPATH += ../..
DESTDIR = ..
DEFINES += IMPORTXML_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += importxml.cpp
HEADERS += importxml.h \
    ../../interfaces.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    imports.json
