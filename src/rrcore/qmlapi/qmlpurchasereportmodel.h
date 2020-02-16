#ifndef QMLPURCHASEREPORTMODEL_H
#define QMLPURCHASEREPORTMODEL_H

#include "models/abstractreportmodel.h"
#include "utility/purchase/purchasereporttransaction.h"

class QMLPurchaseReportModel : public AbstractReportModel
{
    Q_OBJECT
public:
    enum Roles {
        CategoryRole = Qt::UserRole,
        ProductRole,
        QuantityBoughtRole,
        TotalAmountRole,
        UnitRole
    };

    enum SuccessCode {
        ViewPurchaseReportSuccess
    }; Q_ENUM(SuccessCode)

    enum Columns {
        CategoryColumn,
        ProductColumn,
        QuantityBoughtColumn,
        TotalAmountColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    explicit QMLPurchaseReportModel(QObject *parent = nullptr);
    explicit QMLPurchaseReportModel(DatabaseThread &thread, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    Utility::PurchaseReportTransactionList m_transactions;
};

#endif // QMLPURCHASEREPORTMODEL_H
