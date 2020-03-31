#include "qmlexpensetransactionmodel.h"
#include "database/databasethread.h"
#include "queryexecutors/expense.h"

QMLExpenseTransactionModel::QMLExpenseTransactionModel(QObject *parent) :
    QMLExpenseTransactionModel (DatabaseThread::instance(), parent)
{}

QMLExpenseTransactionModel::QMLExpenseTransactionModel(DatabaseThread &thread,
                                                       QObject *parent) :
    AbstractTransactionModel (thread, parent)
{}

int QMLExpenseTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_transactions.count();
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
        return m_transactions.at(index.row()).id;
    case ClientIdRole:
        return m_transactions.at(index.row()).client.id;
    case ClientNameRole:
        return m_transactions.at(index.row()).client.preferredName;
    case AmountRole:
        return m_transactions.at(index.row()).amount.toDouble();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLExpenseTransactionModel::roleNames() const
{
    return {
        { TransactionIdRole, "transaction_id" },
        { ClientIdRole, "client_id" },
        { ClientNameRole, "client_name" },
        { AmountRole, "amount" }
    };
}

QVariant QMLExpenseTransactionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case TransactionIdRole:
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
    emit execute(new ExpenseQuery::ViewExpenseTransactions(dateTimeSpan(),
                                                           Utility::RecordGroup::None,
                                                           this));
}

bool QMLExpenseTransactionModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLExpenseTransactionModel::processResult(const QueryResult &result)
{
    setBusy(false);
    beginResetModel();
    if (result.isSuccessful()) {
        m_transactions = Utility::ExpenseTransactionList{ result.outcome().toMap().value("transactions").toList() };
        emit success();
    } else {
        m_transactions.clear();
        emit error();
    }
    endResetModel();
}
