#ifndef QMLSALEREPORTMODEL_H
#define QMLSALEREPORTMODEL_H

#include "models/abstractreportmodel.h"
#include "utility/sales/salereporttransaction.h"

class QMLSaleReportModel : public AbstractReportModel
{
    Q_OBJECT
public:
    enum Roles {
        CategoryRole = Qt::UserRole,
        ProductRole,
        QuantitySoldRole,
        TotalRevenueRole,
        UnitRole
    };

    enum Columns {
        CategoryColumn,
        ProductColumn,
        QuantitySoldColumn,
        TotalRevenueColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    explicit QMLSaleReportModel(QObject *parent = nullptr);
    explicit QMLSaleReportModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &index = QModelIndex()) const override;
    int columnCount(const QModelIndex &index = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult &result) const override final;
    void processResult(const QueryResult &result) override final;
private:
    Utility::SaleReportTransactionList m_transactions;
};

#endif // QMLSALEREPORTMODEL_H
