#include "qmlexpensetransactionmodel.h"

#include "queryexecutors/expense.h"

QMLExpenseTransactionModel::QMLExpenseTransactionModel(QObject *parent) :
    AbstractTransactionModel (parent)
{

}

QMLExpenseTransactionModel::QMLExpenseTransactionModel(DatabaseThread &thread) :
    AbstractTransactionModel (thread)
{

}

int QMLExpenseTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int QMLExpenseTransactionModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QVariant QMLExpenseTransactionModel::data(const QModelIndex &index, int role) const
{
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

QHash<int, QByteArray> QMLExpenseTransactionModel::roleNames() const
{
    return {
        { TransactionIdRole, "transaction_id" },
        { ClientNameRole, "client_name" },
        { AmountRole, "amount" }
    };
}

QVariant QMLExpenseTransactionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case TransactionIdColumn:
                return tr("Transaction ID");
            case ClientNameColumn:
                return tr("Client name");
            case AmountColumn:
                return tr("Amount");
            case ActionColumn:
                return tr("Action");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case TransactionIdColumn:
                return Qt::AlignRight;
            case ClientNameColumn:
                return Qt::AlignLeft;
            case AmountColumn:
                return Qt::AlignRight;
            case ActionColumn:
                return Qt::AlignHCenter;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
            case TransactionIdColumn:
                return 120;
            case ClientNameColumn:
                return 330;
            case AmountColumn:
                return 160;
            case ActionColumn:
                return 125;
            }
        }
    }

    return section + 1;
}

void QMLExpenseTransactionModel::tryQuery()
{
    setBusy(true);
    emit execute(new ExpenseQuery::ViewExpenseTransactions(
                     QDateTime(QDate(QDate::currentDate().year(), 1, 1), QTime(12, 0)),
                     QDateTime::currentDateTime(),
                     false,
                     this));
}

void QMLExpenseTransactionModel::processResult(const QueryResult result)
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
