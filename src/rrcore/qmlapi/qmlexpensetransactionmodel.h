#ifndef QMLEXPENSETRANSACTIONMODEL_H
#define QMLEXPENSETRANSACTIONMODEL_H

#include "models/abstracttransactionmodel.h"

class QMLExpenseTransactionModel : public AbstractTransactionModel
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
        ViewExpenseTransactionsSuccess
    }; Q_ENUM(SuccessCode)

    enum Columns {
        TransactionIdColumn,
        ClientNameColumn,
        AmountColumn,
        ActionColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    explicit QMLExpenseTransactionModel(QObject *parent = nullptr);
    explicit QMLExpenseTransactionModel(DatabaseThread &thread);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    QVariantList m_records;
};

#endif // QMLEXPENSETRANSACTIONMODEL_H
