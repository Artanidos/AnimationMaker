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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include "resizeableitem.h"

class TreeItem
{
public:
    explicit TreeItem(const QVariant &data, const QVariant &tem, TreeItem *parentItem = 0, const QVariant &level = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);
    inline QList<TreeItem*> *children() {return m_childItems;}
    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();
    void setData(int column, QVariant value);

private:
    QList<TreeItem*> *m_childItems;
    QVariant m_itemData;
    QVariant m_item;
    QVariant m_level;
    TreeItem *m_parentItem;
};
TreeItem *searchChild(TreeItem *parent, ResizeableItem *item);
TreeItem *searchChild(TreeItem *parent, QString value);

#endif // TREEITEM_H
