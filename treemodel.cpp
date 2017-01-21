#include "treeitem.h"
#include "treemodel.h"

#include <QStringList>
#include <QPixmap>

TreeModel::TreeModel(AnimationScene *scene, QObject *parent)
    : QAbstractItemModel(parent)
{
    setScene(scene);
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

void TreeModel::setScene(AnimationScene *scene)
{
    beginResetModel();
    m_scene = scene;

    QVariant rootData = "Root";
    m_rootItem = new TreeItem(rootData);

    if(scene)
    {
        TreeItem *firstItem = new TreeItem("Scene", m_rootItem);
        m_rootItem->appendChild(firstItem);
        readChildren(scene, firstItem);
    }
    endResetModel();
}

void TreeModel::readChildren(AnimationScene *scene, TreeItem *parent)
{
    /*
    for(int i=0; i < scene->childCount(); i++)
    {
        Item *item = scene->childAt(i);
        QString name= item->getTypeName();
        TreeItem *treeitem = new TreeItem(name, parent);
        parent->appendChild(treeitem);
        //readChildren(item, treeitem);
    }
    */
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
        return item->data(index.column());
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
