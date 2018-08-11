#include "qmlsaletransactionitemmodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"

#include <QDateTime>

QMLSaleTransactionItemModel::QMLSaleTransactionItemModel(QObject *parent) :
    AbstractTransactionItemModel(parent)
{

}

int QMLSaleTransactionItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant QMLSaleTransactionItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TransactionItemIdRole:
        return m_records.at(index.row()).toMap().value("id").toInt();
        break;
    case CategoryIdRole:
        return m_records.at(index.row()).toMap().value("category_id").toInt();
        break;
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
        break;
    case ItemIdRole:
        return m_records.at(index.row()).toMap().value("item_id").toInt();
        break;
    case ItemRole:
        return m_records.at(index.row()).toMap().value("item").toString();
        break;
    case UnitPriceRole:
        return m_records.at(index.row()).toMap().value("unit_price").toDouble();
        break;
    case QuantityRole:
        return m_records.at(index.row()).toMap().value("quantity").toDouble();
        break;
    case UnitIdRole:
        return m_records.at(index.row()).toMap().value("unit_id").toInt();
        break;
    case UnitRole:
        return m_records.at(index.row()).toMap().value("unit").toString();
        break;
    case CostRole:
        return m_records.at(index.row()).toMap().value("cost").toDouble();
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
    case UserRole:
        return m_records.at(index.row()).toMap().value("user").toInt();
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLSaleTransactionItemModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractTransactionItemModel::roleNames());
    roles.insert(TransactionItemIdRole, "transaction_item_id");
    roles.insert(CategoryIdRole, "category_id");
    roles.insert(CategoryRole, "category");
    roles.insert(ItemIdRole, "item_id");
    roles.insert(ItemRole, "item");
    roles.insert(UnitPriceRole, "unit_price");
    roles.insert(QuantityRole, "quantity");
    roles.insert(UnitIdRole, "unit_id");
    roles.insert(UnitRole, "unit");
    roles.insert(CostRole, "cost");
    roles.insert(DiscountRole, "discount");
    roles.insert(CurrencyRole, "currency");
    roles.insert(NoteIdRole, "note_id");
    roles.insert(NoteRole, "note");
    roles.insert(SuspendedRole, "suspended");
    roles.insert(ArchivedRole, "archived");
    roles.insert(CreatedRole, "created");
    roles.insert(LastEditedRole, "last_edited");
    roles.insert(UserIdRole, "user_id");
    roles.insert(UserRole, "user");

    return roles;
}

void QMLSaleTransactionItemModel::refresh()
{
    tryQuery();
}

void QMLSaleTransactionItemModel::tryQuery()
{
    if (transactionId() <= -1)
        return;

    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_sale_transaction_items", { { "transaction_id", transactionId() } }, QueryRequest::Sales);
    emit executeRequest(request);
}

void QMLSaleTransactionItemModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "view_sale_transaction_items") {
            beginResetModel();
            m_records = result.outcome().toMap().value("items").toList();
            endResetModel();

            emit success(ViewSaleTransactionItemsSuccess);
        }
    } else {
        emit error(UnknownError);
    }
}

void QMLSaleTransactionItemModel::removeTransactionItem(int row)
{
    setBusy(true);
    QueryRequest request(this);
    QVariantMap params;
    params.insert("can_undo", true);
    params.insert("transaction_id", transactionId());
    params.insert("transaction_item_id", data(index(row), TransactionItemIdRole).toInt());

    request.setCommand("remove_sale_transaction_item", params, QueryRequest::Sales);
    emit executeRequest(request);
}
