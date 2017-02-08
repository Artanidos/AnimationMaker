#ifndef TIMELINEMODEL_H
#define TIMELINEMODEL_H

#include <QAbstractItemModel>
#include <QGraphicsItem>
#include "treeitem.h"
#include "resizeableitem.h"

class TimelineModel : public QAbstractItemModel
{
public:
    TimelineModel();

    //void addItemToAnimate(QGraphicsItem *item);
    //void addProperty(QString text, QModelIndex index);
    void addPropertyAnimation(ResizeableItem *item, QString propertyName);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

private:
    TreeItem *m_rootItem = NULL;
};

#endif // TIMELINEMODEL_H
