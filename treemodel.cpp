#include "treeitem.h"
#include "treemodel.h"
#include "rectangle.h"
#include "ellipse.h"
#include "text.h"
#include "bitmap.h"
#include "vectorgraphic.h"

#include <QStringList>
#include <QPixmap>
#include <QGraphicsItem>
#include <QtTest/QTest>

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
        QString typeName = getItemTypeName(item);
        if(!typeName.isEmpty())
        {
            TreeItem *treeitem = new TreeItem(typeName, qVariantFromValue((void *) item), parent);
            parent->appendChild(treeitem);
        }
    }
}

QString TreeModel::getItemTypeName(QGraphicsItem *item)
{
    switch(item->type())
    {
        case Rectangle::Type:
        {
            return QString("Rectangle");
        }
        case Ellipse::Type:
        {
            return QString("Ellipse");
        }
        case Text::Type:
        {
            return QString("Text");
        }
        case Bitmap::Type:
        {
            return QString("Bitmap");
        }
        case Vectorgraphic::Type:
        {
            return QString("Vectorgraphic");
        }
        default:
        qWarning() << "unknown item type: " << item->type();
        break;
    }
    return QString();
}

void TreeModel::addItem(QGraphicsItem *item)
{
    TreeItem *treeItem = new TreeItem(getItemTypeName(item), qVariantFromValue((void *) item), m_rootItem->child(0));
    m_rootItem->child(0)->appendChild(treeItem);
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
