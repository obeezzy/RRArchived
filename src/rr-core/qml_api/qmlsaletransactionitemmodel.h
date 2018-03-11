#ifndef QMLSALETRANSACTIONITEMMODEL_H
#define QMLSALETRANSACTIONITEMMODEL_H

#include "rr-core/models/abstractvisuallistmodel.h"
#include <QSqlRecord>

class QMLSaleTransactionItemModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Roles {
        CategoryIdRole = Qt::UserRole,
        CategoryRole,
        ItemIdRole,
        ItemRole,
        QuantityRole,
        UnitIdRole,
        UnitRole,
        RetailPriceRole,
        TotalCostRole,
        AmountPaidRole,
        BalanceRole,
        CreatedRole,
        LastEditedRole,
        UserIdRole
    };

    explicit QMLSaleTransactionItemModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
protected:
    void tryQuery() override final;
    void processResult(const QueryResult &result) override final;
private:
    QList<QSqlRecord> m_records;
};

#endif // QMLSALETRANSACTIONITEMMODEL_H
