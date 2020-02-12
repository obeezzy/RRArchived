#include "qmlsaletransactionitemmodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "queryexecutors/sales.h"
#include "database/databasethread.h"

#include <QDateTime>

QMLSaleTransactionItemModel::QMLSaleTransactionItemModel(QObject *parent) :
    AbstractTransactionItemModel(parent)
{

}

QMLSaleTransactionItemModel::QMLSaleTransactionItemModel(DatabaseThread &thread, QObject *parent) :
    AbstractTransactionItemModel(thread, parent)
{

}

int QMLSaleTransactionItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int QMLSaleTransactionItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QVariant QMLSaleTransactionItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case PurchasedProductIdRole:
        return m_records.at(index.row()).toMap().value("purchased_product_id").toInt();
    case CategoryIdRole:
        return m_records.at(index.row()).toMap().value("category_id").toInt();
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
    case ProductIdRole:
        return m_records.at(index.row()).toMap().value("product_id").toInt();
    case ProductRole:
        return m_records.at(index.row()).toMap().value("product").toString();
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

QHash<int, QByteArray> QMLSaleTransactionItemModel::roleNames() const
{
    return {
        { PurchasedProductIdRole, "purchased_product_id" },
        { CategoryIdRole, "category_id" },
        { CategoryRole, "category" },
        { ProductIdRole, "product_id" },
        { ProductRole, "product" },
        { UnitPriceRole, "unit_price" },
        { QuantityRole, "quantity" },
        { UnitIdRole, "unit_id" },
        { UnitRole, "unit" },
        { CostRole, "cost" },
        { DiscountRole, "discount" },
        { CurrencyRole, "currency" },
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

QVariant QMLSaleTransactionItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case CategoryColumn:
                return tr("Category");
            case ProductColumn:
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
            case ProductColumn:
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
            case ProductColumn:
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

void QMLSaleTransactionItemModel::tryQuery()
{
    if (transactionId() <= -1)
        return;

    setBusy(true);
    emit execute(new SaleQuery::ViewSoldProducts(transactionId(),
                                                 this));
}

void QMLSaleTransactionItemModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == SaleQuery::ViewSoldProducts::COMMAND) {
            beginResetModel();
            m_records = result.outcome().toMap().value("products").toList();
            endResetModel();

            emit success(ViewSaleTransactionItemsSuccess);
        } else {
            emit success();
        }
    } else {
        emit error(UnknownError);
    }
}

QString QMLSaleTransactionItemModel::columnName(int column) const
{
    switch (column) {
    case CategoryColumn:
        return "category";
    case ProductColumn:
        return "product";
    case QuantityColumn:
        return "quantity";
    case UnitPriceColumn:
        return "unit_price";
    case CostColumn:
        return "cost";
    }

    return QString();
}

void QMLSaleTransactionItemModel::removeTransactionItem(int row)
{
    setBusy(true);
    emit execute(new SaleQuery::RemoveSoldProduct(SoldProduct {
                                                      transactionId(),
                                                      data(index(row, 0), PurchasedProductIdRole).toInt()
                                                  }, this));
}
