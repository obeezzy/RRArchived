#ifndef QMLSTOCKREPORTMODEL_H
#define QMLSTOCKREPORTMODEL_H

#include "models/abstractreportmodel.h"
#include "utility/stock/stockreporttransaction.h"

class QMLStockReportModel : public AbstractReportModel
{
    Q_OBJECT
public:
    enum Roles {
        CategoryRole = Qt::UserRole,
        ProductRole,
        OpeningStockQuantityRole,
        QuantitySoldRole,
        QuantityBoughtRole,
        QuantityInStockRole,
        UnitRole
    };

    enum Columns {
        CategoryColumn,
        ProductColumn,
        OpeningStockQuantityColumn,
        QuantitySoldColumn,
        QuantityBoughtColumn,
        QuantityInStockColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    explicit QMLStockReportModel(QObject *parent = nullptr);
    explicit QMLStockReportModel(DatabaseThread &thread,
                                 QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult &result) const override final;
    void processResult(const QueryResult &result) override final;
private:
    Utility::Stock::StockReportTransactionList m_transactions;
};

#endif // QMLSTOCKREPORTMODEL_H
