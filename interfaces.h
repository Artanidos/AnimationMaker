/****************************************************************************
** Copyright (C) 2016 Olaf Japp
**
** This file is part of AnimationMaker.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QStatusBar>
#include <QGraphicsView>
#include <QVariant>

namespace AnimationMaker
{

class Keyframe
{
public:
    int time;
    QVariant value;
    int easing;
    Keyframe *next;
    Keyframe *prev;
};

class AnimationItem
{
public:
    QString id;
    qreal left;
    qreal top;
    int opacity;
    QHash<QString, Keyframe*> keyframes;
    virtual ~AnimationItem() {}
};

class Rectangle : public AnimationItem
{
public:
    qreal width;
    qreal height;
    QString pen;
    QString brush;
};

class Ellipse : public AnimationItem
{
public:
    qreal width;
    qreal height;
    QString pen;
    QString brush;
};

class Text : public AnimationItem
{
public:
    qreal xscale;
    qreal yscale;
    QString text;
    QString textcolor;
    QString fontFamily;
    int fontSize;
    QString fontStyle;
};

class Bitmap : public AnimationItem
{
public:
    qreal width;
    qreal height;
    QImage image;
};

class Vectorgraphic : public AnimationItem
{
public:
    qreal xscale;
    qreal yscale;
    QByteArray data;
};

class Animation
{
public:
    int fps;
    int width;
    int height;
    QList<AnimationItem*> items;
};
}

class ExportMovieInterface : public QObject
{
    Q_OBJECT
public:
    virtual QString displayName() const = 0;
    virtual QString filter() const = 0;
    virtual QString title() const = 0;
    virtual void exportMovie(QString filename, QGraphicsView *view, int length, int fps, QStatusBar *bar) = 0;
signals:
    void setFrame(int);
};

#define ExportMovieInterface_iid "org.artanidos.AnimationMaker.ExportMovieInterface"
Q_DECLARE_INTERFACE(ExportMovieInterface, ExportMovieInterface_iid)

class ExportMetaInterface : public QObject
{
    Q_OBJECT
public:
    virtual QString displayName() const = 0;
    virtual QString filter() const = 0;
    virtual QString title() const = 0;
    virtual void exportMeta(QString filename, AnimationMaker::Animation *animation, bool exportAll, QStatusBar *bar) = 0;
};

#define ExportMetaInterface_iid "org.artanidos.AnimationMaker.ExportMetaInterface"
Q_DECLARE_INTERFACE(ExportMetaInterface, ExportMetaInterface_iid)

#endif // INTERFACES_H
