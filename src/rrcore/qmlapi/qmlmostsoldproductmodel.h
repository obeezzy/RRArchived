#ifndef QMLMOSTSOLDPRODUCTMODEL_H
#define QMLMOSTSOLDPRODUCTMODEL_H

#include "models/abstractkpimodel.h"
#include <QVariantList>

class DatabaseThread;

class QMLMostSoldProductModel : public AbstractKpiModel
{
    Q_OBJECT
public:
    enum Roles {
        ProductCategoryIdRole = Qt::UserRole,
        ProductCategoryRole,
        ProductIdRole,
        ProductRole,
        TotalRevenueRole,
        TotalQuantityRole,
        ProductUnitIdRole,
        ProductUnitRole
    };

    enum Columns {
        ProductColumn,
        TotalQuantityColumn,
        TotalRevenueColumn,
        ColumnCount
    };

    explicit QMLMostSoldProductModel(QObject *parent = nullptr);
    explicit QMLMostSoldProductModel(DatabaseThread &thread,
                                     QObject *parent = nullptr);
    ~QMLMostSoldProductModel() override = default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
protected:
    void tryQuery() override final;
    bool canProcessResult(const QueryResult &result) const override;
    void processResult(const QueryResult &result) override final;
private:
    QVariantList m_products;
};

#endif // QMLMOSTSOLDPRODUCTMODEL_H
