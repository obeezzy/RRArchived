#ifndef QMLINCOMETRANSACTIONMODEL_H
#define QMLINCOMETRANSACTIONMODEL_H

#include "models/abstracttransactionmodel.h"
#include "utility/incomeutils.h"

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

    enum SuccessCode {
        ViewIncomeTransactionsSuccess
    }; Q_ENUM(SuccessCode)

    enum Columns {
        TransactionIdColumn,
        ClientNameColumn,
        AmountColumn,
        ActionColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    explicit QMLIncomeTransactionModel(QObject *parent = nullptr);
    explicit QMLIncomeTransactionModel(DatabaseThread &thread,
                                       QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    Utility::IncomeTransactionList m_transactions;
};

#endif // QMLINCOMETRANSACTIONMODEL_H
