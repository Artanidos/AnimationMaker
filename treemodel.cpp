#include "treeitem.h"
#include "treemodel.h"

#include <QStringList>
#include <QPixmap>

TreeModel::TreeModel(QObject *root, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_root = root;
    QVariant rootData = "Root";
    m_rootItem = new TreeItem(rootData);
    TreeItem *firstItem = new TreeItem(getTypeName(m_root->metaObject()->className(), m_root->objectName()), m_rootItem);
    m_rootItem->appendChild(firstItem);
    readChildren(m_root, firstItem);
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

QString TreeModel::getTypeName(QString classname, QString objectName)
{
    if(!objectName.isEmpty())
        return objectName;
    if(classname.contains("QQuick"))
        classname.remove("QQuick");
    if(classname.contains("_"))
        classname.remove(QRegExp("_[0-9]*.*"));
    return classname;
}

void TreeModel::readChildren(QObject *object, TreeItem *parent)
{
    foreach(QObject *item, object->children())
    {
        TreeItem *treeitem = new TreeItem(getTypeName(item->metaObject()->className(), item->objectName()), parent);
        parent->appendChild(treeitem);
        readChildren(item, treeitem);
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
