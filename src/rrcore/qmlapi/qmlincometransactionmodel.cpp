#include "qmlincometransactionmodel.h"

const int COLUMN_COUNT = 4;

QMLIncomeTransactionModel::QMLIncomeTransactionModel(QObject *parent) :
    AbstractTransactionModel (parent)
{

}

QMLIncomeTransactionModel::QMLIncomeTransactionModel(DatabaseThread &thread) :
    AbstractTransactionModel (thread)
{

}

int QMLIncomeTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int QMLIncomeTransactionModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return COLUMN_COUNT;
}

QVariant QMLIncomeTransactionModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TransactionIdRole:
        return m_records.at(index.row()).toMap().value("transaction_id").toInt();
    case ClientNameRole:
        return m_records.at(index.row()).toMap().value("client_name").toString();
    case AmountRole:
        return m_records.at(index.row()).toMap().value("amount").toDouble();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLIncomeTransactionModel::roleNames() const
{
    return {
        { TransactionIdRole, "transaction_id" },
        { ClientNameRole, "client_name" },
        { AmountRole, "amount" }
    };
}

void QMLIncomeTransactionModel::tryQuery()
{
    setBusy(true);

    QueryRequest request(this);
    request.setCommand("view_income_transactions", { { "archived", false } }, QueryRequest::Income);
    emit executeRequest(request);
}

void QMLIncomeTransactionModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    beginResetModel();
    if (result.isSuccessful()) {
        m_records = result.outcome().toMap().value("transactions").toList();
        emit success();
    } else {
        m_records.clear();
        emit error();
    }
    endResetModel();
}
