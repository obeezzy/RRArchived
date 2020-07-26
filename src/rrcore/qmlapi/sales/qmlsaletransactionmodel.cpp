#include "qmlsaletransactionmodel.h"
#include "database/databasethread.h"
#include "query/sales/viewsaletransactions.h"
#include "utility/common/datetimespan.h"
#include <QDateTime>

QMLSaleTransactionModel::QMLSaleTransactionModel(QObject *parent) :
    QMLSaleTransactionModel(DatabaseThread::instance(), parent)
{}

QMLSaleTransactionModel::QMLSaleTransactionModel(DatabaseThread &thread,
                                                 QObject *parent) :
    AbstractTransactionModel(thread, parent)
{}

QVariant QMLSaleTransactionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case SaleTransactionIdRole:
        return m_transactions.at(index.row()).id;
    case CustomerIdRole:
        return m_transactions.at(index.row()).customer.id;
    case CustomerNameRole:
        return m_transactions.at(index.row()).customer.client.preferredName;
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
    case UserRole:
        return m_transactions.at(index.row()).user.user;
    }

    return QVariant();
}

int QMLSaleTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_transactions.count();
}

int QMLSaleTransactionModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QHash<int, QByteArray> QMLSaleTransactionModel::roleNames() const
{
    return {
        { SaleTransactionIdRole, "sale_transaction_id" },
        { CustomerIdRole, "customer_id" },
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
        { UserIdRole, "user_id" },
        { UserRole, "user" }
    };
}

QVariant QMLSaleTransactionModel::headerData(int section, Qt::Orientation orientation, int role) const
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

void QMLSaleTransactionModel::tryQuery()
{
    setBusy(true);
    Utility::RecordGroup::Flags flags;

    if (keys() == Suspended)
        flags.setFlag(Utility::RecordGroup::Suspended);
    else if (keys() == Archived)
        flags.setFlag(Utility::RecordGroup::Archived);
    else if (keys() == All)
        flags = Utility::RecordGroup::None;

    emit execute(new Query::Sales::ViewSaleTransactions(dateTimeSpan(),
                                                         flags,
                                                         this));
}

bool QMLSaleTransactionModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLSaleTransactionModel::processResult(const QueryResult &result)
{
    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == Query::Sales::ViewSaleTransactions::COMMAND) {
            beginResetModel();
            m_transactions = Utility::Sales::SaleTransactionList{ result.outcome().toMap().value("transactions").toList() };
            endResetModel();

            emit success(ModelResult{ ViewTransactionSuccess });
        } else {
            emit success();
        }
    } else {
        emit error();
    }
}
