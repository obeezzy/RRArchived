#include "qmlpurchasetransactionmodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "utility/purchaseutils.h"
#include "queryexecutors/purchase.h"
#include <QDateTime>

QMLPurchaseTransactionModel::QMLPurchaseTransactionModel(QObject *parent) :
    QMLPurchaseTransactionModel(DatabaseThread::instance(), parent)
{}

QMLPurchaseTransactionModel::QMLPurchaseTransactionModel(DatabaseThread &thread, QObject *parent) :
    AbstractTransactionModel(thread, parent)
{}

QVariant QMLPurchaseTransactionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TransactionIdRole:
        return m_transactions.at(index.row()).id;
    case ClientIdRole:
        return m_transactions.at(index.row()).vendor.client.id;
    case CustomerNameRole:
        return m_transactions.at(index.row()).vendor.client.preferredName;
    case TotalCostRole:
        return m_transactions.at(index.row()).monies.totalCost.toDouble();
    case AmountPaidRole:
        return m_transactions.at(index.row()).monies.amountPaid.toDouble();
    case BalanceRole:
        return m_transactions.at(index.row()).monies.balance.toDouble();
    case DiscountRole:
        return m_transactions.at(index.row()).monies.discount.toDouble();
    case NoteIdRole:
        return m_transactions.at(index.row()).note.id;
    case NoteRole:
        return m_transactions.at(index.row()).note.note;
    case SuspendedRole:
        return m_transactions.at(index.row()).flags.testFlag(Utility::RecordGroup::Suspended);
    case ArchivedRole:
        return m_transactions.at(index.row()).flags.testFlag(Utility::RecordGroup::Archived);
    case CreatedRole:
        return m_transactions.at(index.row()).timestamp.created;
    case LastEditedRole:
        return m_transactions.at(index.row()).timestamp.lastEdited;
    case UserIdRole:
        return m_transactions.at(index.row()).user.id;
    }

    return QVariant();
}

int QMLPurchaseTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_transactions.count();
}

int QMLPurchaseTransactionModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QHash<int, QByteArray> QMLPurchaseTransactionModel::roleNames() const
{
    return {
        { TransactionIdRole, "transaction_id" },
        { ClientIdRole, "client_id" },
        { CustomerNameRole, "customer_name" },
        { TotalCostRole, "total_cost" },
        { AmountPaidRole, "amount_paid" },
        { BalanceRole, "balance" },
        { DiscountRole, "discount" },
        { NoteIdRole, "note_id" },
        { NoteRole, "note" },
        { SuspendedRole, "suspended" },
        { ArchivedRole, "archived" },
        { CreatedRole, "created" },
        { LastEditedRole, "last_edited" },
        { UserIdRole, "user_id" }
    };
}

QVariant QMLPurchaseTransactionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case TransactionIdColumn:
                return tr("ID");
            case CustomerNameColumn:
                return tr("Customer name");
            case TotalCostColumn:
                return tr("Total cost");
            case ActionColumn:
                return tr("Action");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case CustomerNameColumn:
                return Qt::AlignLeft;
            case TransactionIdColumn:
            case TotalCostColumn:
                return Qt::AlignRight;
            case ActionColumn:
                return Qt::AlignHCenter;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
            case TransactionIdColumn:
                return 120;
            case CustomerNameColumn:
                return tableViewWidth() - 120 - 120 - 130;
            case TotalCostColumn:
                return 120;
            case ActionColumn:
                return 130;
            }
        }
    }

    return section + 1;
}

void QMLPurchaseTransactionModel::tryQuery()
{
    setBusy(true);

    Utility::RecordGroup::Flags flags;
    if (keys() == Suspended)
        flags.setFlag(Utility::RecordGroup::Suspended);
    else if (keys() == Archived)
        flags.setFlag(Utility::RecordGroup::Archived);
    else if (keys() == All)
        flags.setFlag(Utility::RecordGroup::None);

    emit execute(new PurchaseQuery::ViewPurchaseTransactions(dateTimeSpan(),
                                                             flags,
                                                             this));
}

bool QMLPurchaseTransactionModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLPurchaseTransactionModel::processResult(const QueryResult &result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == PurchaseQuery::ViewPurchaseTransactions::COMMAND) {
            beginResetModel();
            m_transactions = Utility::PurchaseTransactionList{ result.outcome().toMap().value("transactions").toList() };
            endResetModel();

            emit success(ModelResult{ ViewTransactionSuccess });
        } else if (result.request().command() == PurchaseQuery::RemovePurchaseTransaction::COMMAND) {
            Utility::PurchaseTransaction transaction{ result.request().params() };
            removeTransactionFromModel(transaction);
            emit success(ModelResult{ RemoveTransactionSuccess });
        } else if (result.request().command() == PurchaseQuery::RemovePurchaseTransaction::UNDO_COMMAND) {
            Utility::PurchaseTransaction transaction{ result.request().params() };
            undoRemoveTransactionFromModel(transaction);
            emit success(ModelResult{ UndoRemoveTransactionSuccess });
        } else {
            emit success();
        }
    } else {
        if (result.request().command() == PurchaseQuery::RemovePurchaseTransaction::COMMAND) {
            emit error(ModelResult{ RemoveTransactionError });
        } else if (result.request().command() == PurchaseQuery::RemovePurchaseTransaction::UNDO_COMMAND) {
            emit error(ModelResult{ UndoRemoveTransactionError });
        } else {
            emit error();
        }
    }
}

void QMLPurchaseTransactionModel::removeTransaction(int row)
{
    setBusy(true);
    Utility::PurchaseTransaction &transaction{ m_transactions[row] };
    transaction.row = row;
    emit execute(new PurchaseQuery::RemovePurchaseTransaction(transaction,
                                                              this));
}

void QMLPurchaseTransactionModel::removeTransactionFromModel(const Utility::PurchaseTransaction &transaction)
{
    if (transaction.row < 0 && transaction.row >= rowCount())
        return;

    beginRemoveRows(QModelIndex(), transaction.row, transaction.row);
    m_transactions.removeAt(transaction.row);
    endRemoveRows();
}

void QMLPurchaseTransactionModel::undoRemoveTransactionFromModel(const Utility::PurchaseTransaction &transaction)
{
    if (transaction.row < 0 && transaction.row >= rowCount())
        return;

    beginInsertRows(QModelIndex(), transaction.row, transaction.row);
    m_transactions.insert(transaction.row, transaction);
    endInsertRows();
}
