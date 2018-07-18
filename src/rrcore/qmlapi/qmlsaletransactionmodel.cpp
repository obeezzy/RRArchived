#include "qmlsaletransactionmodel.h"
#include <QSqlRecord>
#include <QDateTime>

#include "database/queryrequest.h"
#include "database/queryresult.h"

QMLSaleTransactionModel::QMLSaleTransactionModel(QObject *parent) :
    AbstractTransactionModel(parent)
{
}

QVariant QMLSaleTransactionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TransactionIdRole:
        return m_records.at(index.row()).toMap().value("transaction_id").toInt();
        break;
    case ClientIdRole:
        return m_records.at(index.row()).toMap().value("client_id").toInt();
        break;
    case CustomerNameRole:
        return m_records.at(index.row()).toMap().value("customer_name").toString();
        break;
    case TotalCostRole:
        return m_records.at(index.row()).toMap().value("total_cost").toDouble();
        break;
    case AmountPaidRole:
        return m_records.at(index.row()).toMap().value("amount_paid").toDouble();
        break;
    case BalanceRole:
        return m_records.at(index.row()).toMap().value("balance").toDouble();
        break;
    case DiscountRole:
        return m_records.at(index.row()).toMap().value("discount").toDouble();
        break;
    case NoteIdRole:
        return m_records.at(index.row()).toMap().value("note_id").toInt();
        break;
    case NoteRole:
        return m_records.at(index.row()).toMap().value("note").toString();
        break;
    case SuspendedRole:
        return m_records.at(index.row()).toMap().value("suspended").toBool();
        break;
    case ArchivedRole:
        return m_records.at(index.row()).toMap().value("archived").toBool();
        break;
    case CreatedRole:
        return m_records.at(index.row()).toMap().value("created").toDateTime();
        break;
    case LastEditedRole:
        return m_records.at(index.row()).toMap().value("last_edited").toDateTime();
        break;
    case UserIdRole:
        return m_records.at(index.row()).toMap().value("user_id").toInt();
        break;
    case Qt::DisplayRole:
        return m_records.at(index.row()).toMap();
        break;
    }

    return QVariant();
}

int QMLSaleTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QHash<int, QByteArray> QMLSaleTransactionModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractTransactionModel::roleNames());
    roles.insert(TransactionIdRole, "transaction_id");
    roles.insert(ClientIdRole, "client_id");
    roles.insert(CustomerNameRole, "customer_name");
    roles.insert(TotalCostRole, "total_cost");
    roles.insert(AmountPaidRole, "amount_paid");
    roles.insert(BalanceRole, "balance");
    roles.insert(DiscountRole, "discount");
    roles.insert(NoteIdRole, "note_id");
    roles.insert(NoteRole, "note");
    roles.insert(SuspendedRole, "suspended");
    roles.insert(ArchivedRole, "archived");
    roles.insert(CreatedRole, "created");
    roles.insert(LastEditedRole, "last_edited");
    roles.insert(UserIdRole, "user_id");

    return roles;
}

void QMLSaleTransactionModel::tryQuery()
{
    setBusy(true);
    QueryRequest request(this);
    QVariantMap params;

    if (keys() == Completed) {
        params.insert("suspended", false);
        params.insert("archived", false);
    } else if (keys() == Suspended) {
        params.insert("suspended", true);
        params.insert("archived", false);
    } else if (keys() == Archived) {
        params.insert("suspended", false);
        params.insert("archived", true);
    } else if (keys() == All) {
        params.insert("suspended", true);
        params.insert("archived", true);
    }

    if (!from().isNull() && from().isValid())
        params.insert("from", from());
    if (!to().isNull() && to().isValid())
        params.insert("to", to());

    request.setCommand("view_sale_transactions", params, QueryRequest::Sales);
    emit executeRequest(request);
}

void QMLSaleTransactionModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == "view_sale_transactions") {
            beginResetModel();
            m_records = result.outcome().toMap().value("transactions").toList();
            endResetModel();

            emit success(ViewTransactionSuccess);
        } else {
            emit success(UnknownSuccess);
        }
    } else {
        emit error(UnknownError);
    }
}

void QMLSaleTransactionModel::removeTransaction(int row)
{
    setBusy(true);
    QueryRequest request(this);
    QVariantMap params;
    params.insert("can_undo", true);
    params.insert("transaction_id", data(index(row), TransactionIdRole).toInt());

    request.setCommand("remove_transaction", params, QueryRequest::Sales);
    emit executeRequest(request);
}
