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

#include "treeitem.h"
#include "treemodel.h"
#include "rectangle.h"
#include "ellipse.h"
#include "text.h"
#include "bitmap.h"
#include "vectorgraphic.h"
#include "animationscene.h"

#include <QStringList>
#include <QPixmap>
#include <QGraphicsItem>
#include <QtTest/QTest>
#include <QModelIndex>

TreeModel::TreeModel(AnimationScene *scene, QObject *parent)
    : QAbstractItemModel(parent)
{
    setScene(scene);
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

void TreeModel::reset()
{
    beginResetModel();
    m_rootItem->child(0)->children()->clear();
    endResetModel();
}

void TreeModel::setScene(AnimationScene *scene)
{
    beginResetModel();
    m_scene = scene;

    QVariant rootData = "Root";
    QVariant item;
    if(m_rootItem)
        delete m_rootItem;
    m_rootItem = new TreeItem(rootData, item);

    if(scene)
    {
        TreeItem *firstItem = new TreeItem("Scene", item, m_rootItem);
        m_rootItem->appendChild(firstItem);
        readChildren(scene, firstItem);
    }
    endResetModel();
}

void TreeModel::readChildren(AnimationScene *scene, TreeItem *parent)
{
    QList<QGraphicsItem*> itemList = scene->items(Qt::AscendingOrder);
    foreach (QGraphicsItem *item, itemList)
    {
        ResizeableItem *ri = dynamic_cast<ResizeableItem*>(item);
        if(ri)
        {
            TreeItem *treeitem = new TreeItem(ri->id(), qVariantFromValue((void *) ri), parent);
            parent->appendChild(treeitem);
            connect(ri, SIGNAL(idChanged(ResizeableItem *, QString)), this, SLOT(idChanged(ResizeableItem *, QString)));
        }
    }
}

void TreeModel::addItem(ResizeableItem *item)
{
    TreeItem *treeItem = new TreeItem(item->id(), qVariantFromValue((void *) item), m_rootItem->child(0));
    m_rootItem->child(0)->appendChild(treeItem);
    connect(item, SIGNAL(idChanged(ResizeableItem *, QString)), this, SLOT(idChanged(ResizeableItem *, QString)));
}

void TreeModel::removeItem(ResizeableItem *item)
{
    TreeItem *treeItem = searchChild(m_rootItem->child(0), item);
    if(treeItem)
    {
        QModelIndex index = createIndex(treeItem->row(), 0, treeItem->parentItem());
        beginRemoveRows(index.parent(), treeItem->row(), treeItem->row());
        treeItem->parentItem()->children()->removeAt(treeItem->row());
        endRemoveRows();
        delete treeItem;
    }
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return m_rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DecorationRole)
        return QPixmap(":/images/rect.png");

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

    return QVariant();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
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

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
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

void TreeModel::idChanged(ResizeableItem *item, QString value)
{
    TreeItem *treeItem = searchChild(m_rootItem->child(0), item);
    if(treeItem)
    {
        if(value.isEmpty())
            value = getItemTypeName(item);
        treeItem->setData(0, value);
        this->dataChanged(index(0,0), index(1,0));
    }
}
