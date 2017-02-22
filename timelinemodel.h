#ifndef TIMELINEMODEL_H
#define TIMELINEMODEL_H

#include <QAbstractItemModel>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "treeitem.h"
#include "resizeableitem.h"

class TimelineModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TimelineModel();

    void addPropertyAnimation(ResizeableItem *item, QString propertyName, qreal value);
    void addAnimation(ResizeableItem *item, QPropertyAnimation *anim);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QList<QPropertyAnimation*> *getAnimations();

public slots:
    void idChanged(ResizeableItem *item, QString value);

private:
    TreeItem *m_rootItem = NULL;
    QList<QPropertyAnimation*> *m_animations;
};

#endif // TIMELINEMODEL_H
