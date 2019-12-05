#include "qmlpurchasetransactionmodel.h"
#include <QSqlRecord>
#include <QDateTime>

#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "utility/purchaseutils.h"

#include "queryexecutors/purchase.h"

QMLPurchaseTransactionModel::QMLPurchaseTransactionModel(QObject *parent) :
    QMLPurchaseTransactionModel(DatabaseThread::instance(), parent)
{}

QMLPurchaseTransactionModel::QMLPurchaseTransactionModel(DatabaseThread &thread, QObject *parent) :
    AbstractTransactionModel(thread, parent)
{

}

QVariant QMLPurchaseTransactionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TransactionIdRole:
        return m_records.at(index.row()).toMap().value("transaction_id").toInt();
    case ClientIdRole:
        return m_records.at(index.row()).toMap().value("client_id").toInt();
    case CustomerNameRole:
        return m_records.at(index.row()).toMap().value("customer_name").toString();
    case TotalCostRole:
        return m_records.at(index.row()).toMap().value("total_cost").toDouble();
    case AmountPaidRole:
        return m_records.at(index.row()).toMap().value("amount_paid").toDouble();
    case BalanceRole:
        return m_records.at(index.row()).toMap().value("balance").toDouble();
    case DiscountRole:
        return m_records.at(index.row()).toMap().value("discount").toDouble();
    case NoteIdRole:
        return m_records.at(index.row()).toMap().value("note_id").toInt();
    case NoteRole:
        return m_records.at(index.row()).toMap().value("note").toString();
    case SuspendedRole:
        return m_records.at(index.row()).toMap().value("suspended").toBool();
    case ArchivedRole:
        return m_records.at(index.row()).toMap().value("archived").toBool();
    case CreatedRole:
        return m_records.at(index.row()).toMap().value("created").toDateTime();
    case LastEditedRole:
        return m_records.at(index.row()).toMap().value("last_edited").toDateTime();
    case UserIdRole:
        return m_records.at(index.row()).toMap().value("user_id").toInt();
    }

    return QVariant();
}

int QMLPurchaseTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
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
    bool suspended = false;
    bool archived = false;
    QueryRequest request(this);
    QVariantMap params;

    if (keys() == Completed) {
        suspended = false;
        archived = false;
    } else if (keys() == Suspended) {
        suspended = true;
        archived = false;
    } else if (keys() == Archived) {
        suspended = false;
        archived = true;
    } else if (keys() == All) {
        suspended = true;
        archived = true;
    }

    emit execute(new PurchaseQuery::ViewPurchaseTransactions(from(),
                                                             to(),
                                                             suspended,
                                                             archived,
                                                             this));
}

void QMLPurchaseTransactionModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == PurchaseQuery::ViewPurchaseTransactions::COMMAND) {
            beginResetModel();
            m_records = result.outcome().toMap().value("transactions").toList();
            endResetModel();

            emit success(ViewTransactionSuccess);
        } else if (result.request().command() == PurchaseQuery::RemovePurchaseTransaction::COMMAND) {
            removeTransactionFromModel(result.request().params().value("row").toInt());
            emit success(RemoveTransactionSuccess);
        } else if (result.request().command() == PurchaseQuery::RemovePurchaseTransaction::UNDO_COMMAND) {
            undoRemoveTransactionFromModel(result.request().params().value("row").toInt(),
                                           result.request().params().value("record").toMap());
            emit success(UndoRemoveTransactionSuccess);
        } else {
            emit success(UnknownSuccess);
        }
    } else {
        if (result.request().command() == PurchaseQuery::RemovePurchaseTransaction::COMMAND) {
            emit error(RemoveTransactionError);
        } else if (result.request().command() == PurchaseQuery::RemovePurchaseTransaction::UNDO_COMMAND) {
            emit error(UndoRemoveTransactionError);
        } else {
            emit error(UnknownError);
        }
    }
}

void QMLPurchaseTransactionModel::removeTransaction(int row)
{
    setBusy(true);
    emit execute(new PurchaseQuery::RemovePurchaseTransaction(data(index(row, 0), TransactionIdRole).toInt(),
                                                              row,
                                                              PurchaseTransaction{ m_records.at(row).toMap() },
                                                              this));
}

void QMLPurchaseTransactionModel::removeTransactionFromModel(int row)
{
    if (row < 0 && row >= rowCount())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_records.removeAt(row);
    endRemoveRows();
}

void QMLPurchaseTransactionModel::undoRemoveTransactionFromModel(int row, const QVariantMap &record)
{
    if (row < 0 && row >= rowCount())
        return;

    beginInsertRows(QModelIndex(), row, row);
    m_records.insert(row, record);
    endInsertRows();
}
