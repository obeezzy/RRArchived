#include "qmlsaletransactionitemmodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"

QMLSaleTransactionItemModel::QMLSaleTransactionItemModel(QObject *parent) :
    AbstractVisualListModel(parent)
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
    Q_UNUSED(index)
    Q_UNUSED(role)

    return QVariant();
}

QHash<int, QByteArray> QMLSaleTransactionItemModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(CategoryIdRole, "category_id");
    roles.insert(CategoryRole, "category");
    roles.insert(ItemIdRole, "item_id");
    roles.insert(ItemRole, "item");
    roles.insert(QuantityRole, "quantity");
    roles.insert(UnitIdRole, "unit_id");
    roles.insert(UnitRole, "unit");
    roles.insert(RetailPriceRole, "retail_price");
    roles.insert(TotalCostRole, "total_cost");
    roles.insert(AmountPaidRole, "amount_paid");
    roles.insert(BalanceRole, "balance");
    roles.insert(CreatedRole, "created");
    roles.insert(LastEditedRole, "last_edited");
    roles.insert(UserIdRole, "user_id");

    return roles;
}

void QMLSaleTransactionItemModel::tryQuery()
{
    QueryRequest request(this);
    request.setCommand("view_sale_items_for_transaction", QVariantMap(), QueryRequest::Sales);
    emit executeRequest(request);
}

void QMLSaleTransactionItemModel::processResult(const QueryResult &result)
{
    if (result.request().parent() != this)
        return;
}
