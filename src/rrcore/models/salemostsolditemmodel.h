#ifndef SALEMOSTSOLDITEMMODEL_H
#define SALEMOSTSOLDITEMMODEL_H

#include <QObject>
#include "models/abstractvisuallistmodel.h"
#include <QVariantList>

class SaleMostSoldItemModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Roles {
        CategoryIdRole = Qt::UserRole,
        CategoryRole,
        ItemIdRole,
        ItemRole,
        TotalRevenueRole,
        TotalQuantityRole,
        UnitIdRole,
        UnitRole
    };

    explicit SaleMostSoldItemModel(const QVariantList &records, QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
protected:
    void tryQuery() override final;
    void processResult(const QueryResult &result) override final;
private:
    QVariantList m_records;
};

#endif // SALEMOSTSOLDITEMMODEL_H
