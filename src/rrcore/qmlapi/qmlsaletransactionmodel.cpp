#include "qmlsaletransactionmodel.h"
#include <QSqlRecord>
#include <QDateTime>

#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"

#include "qmlsaletransactionmodel.h"

QMLSaleTransactionModel::QMLSaleTransactionModel(QObject *parent) :
    QMLSaleTransactionModel(DatabaseThread::instance(), parent)
{}

QMLSaleTransactionModel::QMLSaleTransactionModel(DatabaseThread &thread, QObject *parent) :
    AbstractTransactionModel(thread, parent)
{

}

QVariant QMLSaleTransactionModel::data(const QModelIndex &index, int role) const
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
    case Qt::DisplayRole:
        return m_records.at(index.row()).toMap();
    }

    return QVariant();
}

int QMLSaleTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int QMLSaleTransactionModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 1;
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
    params.insert("transaction_id", data(index(row, 0), TransactionIdRole).toInt());

    request.setCommand("remove_transaction", params, QueryRequest::Sales);
    emit executeRequest(request);
}
