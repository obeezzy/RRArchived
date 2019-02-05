#include "qmlpurchasetransactionitemmodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"

#include <QDateTime>

QMLPurchaseTransactionItemModel::QMLPurchaseTransactionItemModel(QObject *parent) :
    AbstractTransactionItemModel (parent)
{

}

QMLPurchaseTransactionItemModel::QMLPurchaseTransactionItemModel(DatabaseThread &thread) :
    AbstractTransactionItemModel (thread)
{

}

int QMLPurchaseTransactionItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant QMLPurchaseTransactionItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TransactionItemIdRole:
        return m_records.at(index.row()).toMap().value("id").toInt();
    case CategoryIdRole:
        return m_records.at(index.row()).toMap().value("category_id").toInt();
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
    case ItemIdRole:
        return m_records.at(index.row()).toMap().value("item_id").toInt();
    case ItemRole:
        return m_records.at(index.row()).toMap().value("item").toString();
    case UnitPriceRole:
        return m_records.at(index.row()).toMap().value("unit_price").toDouble();
    case QuantityRole:
        return m_records.at(index.row()).toMap().value("quantity").toDouble();
    case UnitIdRole:
        return m_records.at(index.row()).toMap().value("unit_id").toInt();
    case UnitRole:
        return m_records.at(index.row()).toMap().value("unit").toString();
    case CostRole:
        return m_records.at(index.row()).toMap().value("cost").toDouble();
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
    case UserRole:
        return m_records.at(index.row()).toMap().value("user").toString();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLPurchaseTransactionItemModel::roleNames() const
{
    return {
        { TransactionItemIdRole, "transaction_item_id" },
        { CategoryIdRole, "category_id" },
        { CategoryRole, "category" },
        { ItemIdRole, "item_id" },
        { ItemRole, "item" },
        { UnitPriceRole, "unit_price" },
        { QuantityRole, "quantity" },
        { UnitIdRole, "unit_id" },
        { UnitRole, "unit" },
        { CostRole, "cost" },
        { DiscountRole, "discount" },
        { CurrencyRole, "currency" },
        { NoteIdRole, "note_id" },
        { SuspendedRole, "suspended" },
        { ArchivedRole, "archived" },
        { CreatedRole, "created" },
        { LastEditedRole, "last_edited" },
        { UserIdRole, "user_id" },
        { UserRole, "user" }
    };
}

void QMLPurchaseTransactionItemModel::tryQuery()
{
    if (transactionId() <= -1)
        return;

    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_purchase_transaction_items", { { "transaction_id", transactionId() } }, QueryRequest::Purchase);
    emit executeRequest(request);
}

void QMLPurchaseTransactionItemModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "view_purchase_transaction_items") {
            beginResetModel();
            m_records = result.outcome().toMap().value("items").toList();
            endResetModel();

            emit success(ViewPurchaseTransactionItemsSuccess);
        }
    } else {
        emit error(UnknownError);
    }
}

void QMLPurchaseTransactionItemModel::removeTransactionItem(int row)
{
    setBusy(true);
    QueryRequest request(this);
    QVariantMap params;
    params.insert("can_undo", true);
    params.insert("transaction_id", transactionId());
    params.insert("transaction_item_id", data(index(row), TransactionItemIdRole).toInt());

    request.setCommand("remove_purchase_transaction_item", params, QueryRequest::Purchase);
    emit executeRequest(request);
}

