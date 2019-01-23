#ifndef QMLINCOMETRANSACTIONMODEL_H
#define QMLINCOMETRANSACTIONMODEL_H

#include "models/abstracttransactionmodel.h"

class QMLIncomeTransactionModel : public AbstractTransactionModel
{
    Q_OBJECT
public:
    enum Roles {
        TransactionIdRole = Qt::UserRole,
        ClientIdRole,
        ClientNameRole,
        AmountRole
    };

    explicit QMLIncomeTransactionModel(QObject *parent = nullptr);
    explicit QMLIncomeTransactionModel(DatabaseThread &thread);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    QVariantList m_records;
};

#endif // QMLINCOMETRANSACTIONMODEL_H
