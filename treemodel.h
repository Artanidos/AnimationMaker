#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "animationscene.h"

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(AnimationScene *scene = 0, QObject *parent = 0);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    void setScene(AnimationScene *scene);
    void addItem(QGraphicsItem *item);

private:
    TreeItem *m_rootItem;
    AnimationScene *m_scene;

    void readChildren(AnimationScene *scene, TreeItem *parent);
    QString getItemTypeName(QGraphicsItem *item);
};

#endif // TREEMODEL_H
