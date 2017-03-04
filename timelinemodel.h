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

#ifndef TIMELINEMODEL_H
#define TIMELINEMODEL_H

#include <QAbstractItemModel>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "treeitem.h"
#include "resizeableitem.h"

class TimelineModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TimelineModel();

    void reset();
    void addPropertyAnimation(ResizeableItem *item, QString propertyName, qreal value, int min, int max);
    void addAnimation(ResizeableItem *item, QPropertyAnimation *anim);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QList<QPropertyAnimation*> *getAnimations();

public slots:
    void idChanged(ResizeableItem *item, QString value);

private:
    TreeItem *m_rootItem = NULL;
    QList<QPropertyAnimation*> *m_animations;
};

#endif // TIMELINEMODEL_H
