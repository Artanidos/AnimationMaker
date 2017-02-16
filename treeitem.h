#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

class TreeItem
{
public:
    explicit TreeItem(const QVariant &data, const QVariant &tem, TreeItem *parentItem = 0, const QVariant &level = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);
    inline QList<TreeItem*> children() {return m_childItems;}
    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

private:
    QList<TreeItem*> m_childItems;
    QVariant m_itemData;
    QVariant m_item;
    QVariant m_level;
    TreeItem *m_parentItem;
};

#endif // TREEITEM_H
