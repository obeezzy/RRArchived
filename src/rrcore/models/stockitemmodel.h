#ifndef STOCKITEMMODEL_H
#define STOCKITEMMODEL_H

#include <QObject>
#include <QVariantList>

#include "abstractvisualtablemodel.h"

class StockItemModel : public AbstractVisualTableModel
{
    Q_OBJECT
public:
    explicit StockItemModel(QObject *parent = nullptr);

    enum Roles {
        CategoryIdRole = Qt::UserRole,
        ItemIdRole,
        ItemRole,
        DescriptionRole,
        DivisibleRole,
        ImageSourceRole,
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

    enum Columns {
        ImageColumn,
        ItemColumn,
        QuantityColumn,
        CostPriceColumn,
        ActionColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QString category() const;
    void setCategory(const QString &category);

    int categoryId() const;
    void setCategoryId(int categoryId);

    QVariantList items() const;
    void setItems(const QVariantList &items);

    bool addItem(int itemId, const QVariantMap &itemInfo, Qt::SortOrder sortOrder = Qt::AscendingOrder);
    bool removeItem(int itemId);
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
private:
    QString m_category;
    int m_categoryId;
    QVariantList m_records;
};

#endif // STOCKITEMMODEL_H
