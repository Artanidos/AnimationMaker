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

#include "timelinemodel.h"
#include "animationscene.h"
#include "rectangle.h"
#include <QPixmap>

TimelineModel::TimelineModel()
{
    QVariant rootData = "Root";
    QVariant data;

    m_rootItem = new TreeItem(rootData, data);
    m_animations = new QList<QPropertyAnimation*>();
}

void TimelineModel::reset()
{
    beginResetModel();
    m_rootItem->children()->clear();
    endResetModel();
    m_animations->clear();
}

void TimelineModel::addAnimation(ResizeableItem *item, QPropertyAnimation *anim)
{
    bool found = false;
    beginInsertRows(QModelIndex(), m_rootItem->childCount() - 1, m_rootItem->childCount() - 1);
    TreeItem *treeItem = searchChild(m_rootItem, item);
    if(treeItem)
        found = true;
    else
    {
        treeItem = new TreeItem(getItemName(item), qVariantFromValue((void *) item), m_rootItem, 1);
        connect(item, SIGNAL(idChanged(ResizeableItem *, QString)), this, SLOT(idChanged(ResizeableItem *, QString)));
    }
    TreeItem *treeChildItem = new TreeItem(anim->propertyName(), qVariantFromValue((void *) anim), treeItem, 2);
    treeItem->appendChild(treeChildItem);
    if(!found)
        m_rootItem->appendChild(treeItem);
    endInsertRows();
    m_animations->append(anim);
}

void TimelineModel::addPropertyAnimation(ResizeableItem *item, QString propertyName, qreal value, int min, int max)
{
    // TODO: check if animation for this property already exists
    bool found = false;
    beginInsertRows(QModelIndex(), m_rootItem->childCount() - 1, m_rootItem->childCount() - 1);
    TreeItem *treeItem = searchChild(m_rootItem, item);
    if(treeItem)
        found = true;
    else
    {
        treeItem = new TreeItem(getItemName(item), qVariantFromValue((void *) item), m_rootItem, 1);
        connect(item, SIGNAL(idChanged(ResizeableItem *, QString)), this, SLOT(idChanged(ResizeableItem *, QString)));
    }
    const QByteArray propName(propertyName.toLatin1());
    QPropertyAnimation *anim = new QPropertyAnimation();
    anim->setTargetObject(item);
    anim->setProperty("begin", QVariant(0));
    anim->setProperty("min", QVariant(min));
    anim->setProperty("max", QVariant(max));
    anim->setDuration(1000);
    anim->setPropertyName(propName);
    anim->setStartValue(value);
    anim->setEndValue(value);
    item->addAnimation(anim);
    TreeItem *treeChildItem = new TreeItem(propertyName, qVariantFromValue((void *) anim), treeItem, 2);
    treeItem->appendChild(treeChildItem);
    if(!found)
        m_rootItem->appendChild(treeItem);
    endInsertRows();
    m_animations->append(anim);
}

QList<QPropertyAnimation*> *TimelineModel::getAnimations()
{
    return m_animations;
}

int TimelineModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return m_rootItem->columnCount();
}

QVariant TimelineModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DecorationRole)
        return QVariant();//QPixmap(":/images/rect.png");

    if (role == Qt::DisplayRole)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        return item->data(0);
    }

    if (role == Qt::UserRole)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        return item->data(1);
    }

    if (role == Qt::UserRole + 1)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        return item->data(2);
    }

    return QVariant();
}

Qt::ItemFlags TimelineModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TimelineModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->data(section);

    return QVariant();
}

QModelIndex TimelineModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TimelineModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TimelineModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void TimelineModel::idChanged(ResizeableItem *item, QString value)
{
    TreeItem *treeItem = searchChild(m_rootItem, item);
    if(treeItem)
    {
        if(value.isEmpty())
            value = getItemTypeName(item);
        treeItem->setData(0, value);
        this->dataChanged(index(0,0), index(1,0));
    }
}
