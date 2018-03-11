#ifndef STOCKITEMMODEL_H
#define STOCKITEMMODEL_H

#include <QObject>
#include <QVariantList>
#include "abstractvisuallistmodel.h"

class StockItemModel : public AbstractVisualListModel
{
public:
    explicit StockItemModel(QObject *parent = nullptr);

    enum Roles {
        CategoryIdRole = Qt::UserRole,
        ItemIdRole,
        ItemRole,
        DescriptionRole,
        DivisibleRole,
        ImageRole,
        QuantityRole,
        UnitRole,
        UnitIdRole,
        CostPriceRole,
        RetailPriceRole,
        CurrencyRole,
        CreatedRole,
        LastEditedRole,
        UserRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    QString category() const;
    void setCategory(const QString &category);

    void setItems(const QVariantList &items);
protected:
    void tryQuery() override final;
    void processResult(const QueryResult &result) override final;
private:
    QString m_category;
    QVariantList m_itemRecords;
};

#endif // STOCKITEMMODEL_H
