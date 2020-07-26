#ifndef QMLPRODUCTMODEL_H
#define QMLPRODUCTMODEL_H

#include "models/abstractvisualtablemodel.h"
#include "utility/stock/product.h"

class QMLProductModel : public AbstractVisualTableModel
{
    Q_OBJECT
    Q_PROPERTY(int categoryId READ categoryId WRITE setCategoryId NOTIFY
                   categoryIdChanged)
public:
    enum Roles
    {
        CategoryIdRole = Qt::UserRole,
        CategoryRole,
        ProductIdRole,
        ProductRole,
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

    enum Columns
    {
        ImageColumn,
        ProductColumn,
        QuantityColumn,
        CostPriceColumn,
        ActionColumn,
        ColumnCount
    };
    Q_ENUM(Columns)

    enum SuccessCode
    {
        UnknownSuccess,
        ViewProductsSuccess,
        RemoveProductSuccess,
        UndoRemoveProductSuccess
    };
    Q_ENUM(SuccessCode)

    explicit QMLProductModel(QObject* parent = nullptr);
    explicit QMLProductModel(DatabaseThread& thread, QObject* parent = nullptr);

    int rowCount(
        const QModelIndex& parent = QModelIndex()) const override final;
    int columnCount(
        const QModelIndex& parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    int categoryId() const;
    void setCategoryId(int categoryId);

    Q_INVOKABLE void removeProduct(int row);
signals:
    void categoryIdChanged();
    void productRemoved(int productId);

protected:
    void tryQuery() override final;
    bool canProcessResult(const QueryResult& result) const override final;
    void processResult(const QueryResult& result) override final;
    QString columnName(int column) const override final;
    void filter() override final;
public slots:
    void refresh() override;
    void undoLastCommit() override;

private:
    int m_categoryId{0};
    Utility::Stock::ProductList m_products;

    void removeProductFromModel(int row);
    void undoRemoveProductFromModel(const Utility::Stock::Product& product);
};

#endif  // QMLPRODUCTMODEL_H
