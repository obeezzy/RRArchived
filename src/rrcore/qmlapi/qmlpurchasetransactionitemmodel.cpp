#include "qmlpurchasetransactionitemmodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "queryexecutors/purchase.h"

#include <QDateTime>

QMLPurchaseTransactionItemModel::QMLPurchaseTransactionItemModel(QObject *parent) :
    QMLPurchaseTransactionItemModel(DatabaseThread::instance(), parent)
{}

QMLPurchaseTransactionItemModel::QMLPurchaseTransactionItemModel(DatabaseThread &thread, QObject *parent) :
    AbstractTransactionItemModel(thread, parent)
{

}

int QMLPurchaseTransactionItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int QMLPurchaseTransactionItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
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

QVariant QMLPurchaseTransactionItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case CategoryColumn:
                return tr("Category");
            case ItemColumn:
                return tr("Item");
            case QuantityColumn:
                return tr("Qty");
            case UnitPriceColumn:
                return tr("Unit price");
            case CostColumn:
                return tr("Cost");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case CategoryColumn:
            case ItemColumn:
                return Qt::AlignLeft;
            case QuantityColumn:
            case UnitPriceColumn:
            case CostColumn:
                return Qt::AlignRight;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
            case CategoryColumn:
                return 120;
            case ItemColumn:
                return tableViewWidth() - 120 - 120 - 120 - 120;
            case QuantityColumn:
                return 120;
            case UnitPriceColumn:
                return 120;
            case CostColumn:
                return 120;
            }
        }
    }

    return section + 1;
}

void QMLPurchaseTransactionItemModel::tryQuery()
{
    if (transactionId() <= -1)
        return;

    setBusy(true);
    emit execute(new PurchaseQuery::ViewPurchaseTransactionItems(transactionId(),
                                                                 this));
}

void QMLPurchaseTransactionItemModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == PurchaseQuery::ViewPurchaseTransactionItems::COMMAND) {
            beginResetModel();
            m_records = result.outcome().toMap().value("items").toList();
            endResetModel();

            emit success(ViewPurchaseTransactionItemsSuccess);
        }
    } else {
        emit error(UnknownError);
    }
}

QString QMLPurchaseTransactionItemModel::columnName(int column) const
{
    switch (column) {
    case CategoryColumn:
        return "category";
    case ItemColumn:
        return "item";
    case QuantityColumn:
        return "quantity";
    case UnitPriceColumn:
        return "unit_price";
    case CostColumn:
        return "cost";
    }

    return QString();
}

void QMLPurchaseTransactionItemModel::removeTransactionItem(int row)
{
    setBusy(true);
    emit execute(new PurchaseQuery::RemovePurchaseTransactionItem(transactionId(),
                                                                  data(index(row, 0), TransactionItemIdRole).toInt(),
                                                                  this));
}

