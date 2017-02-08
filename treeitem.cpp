#include <QStringList>

#include "treeitem.h"

TreeItem::TreeItem(const QVariant &data, const QVariant &item, TreeItem *parent, const QVariant &level)
{
    m_parentItem = parent;
    m_itemData = data;
    m_item = item;
    m_level = level;
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    return m_childItems.value(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return 1;
}

QVariant TreeItem::data(int column) const
{
    if(column == 0)
        return m_itemData;
    else if(column == 1)
        return m_item;
    else if(column == 2)
        return m_level;
    return QVariant();
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
