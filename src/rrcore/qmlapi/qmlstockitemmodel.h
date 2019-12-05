#ifndef QMLSTOCKITEMMODEL_H
#define QMLSTOCKITEMMODEL_H

#include "models/abstractvisualtablemodel.h"

class QMLStockItemModel : public AbstractVisualTableModel
{
    Q_OBJECT
    Q_PROPERTY(int categoryId READ categoryId WRITE setCategoryId NOTIFY categoryIdChanged)
public:
    enum Roles {
        CategoryIdRole = Qt::UserRole,
        CategoryRole,
        ItemIdRole,
        ItemRole,
        DescriptionRole,
        DivisibleRole,
        ImageUrlRole,
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

    enum SuccessCode {
        UnknownSuccess,
        ViewStockItemsSuccess,
        RemoveItemSuccess,
        UndoRemoveItemSuccess
    }; Q_ENUM(SuccessCode)

    explicit QMLStockItemModel(QObject *parent = nullptr);
    explicit QMLStockItemModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int categoryId() const;
    void setCategoryId(int categoryId);

    Q_INVOKABLE void removeItem(int row);
signals:
    void categoryIdChanged();
    void itemRemoved(int itemId);
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
    QString columnName(int column) const override final;
    void filter() override final;
public slots:
    void refresh() override;
private:
    int m_categoryId;
    QVariantList m_records;

    void removeItemFromModel(int row);
    void undoRemoveItemFromModel(int row, const QVariantMap &itemInfo);
};

#endif // QMLSTOCKITEMMODEL_H
