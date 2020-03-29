#include "qmlpurchasetransactionitemmodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "queryexecutors/purchase.h"
#include "utility/purchaseutils.h"
#include <QDateTime>

QMLPurchaseTransactionItemModel::QMLPurchaseTransactionItemModel(QObject *parent) :
    QMLPurchaseTransactionItemModel(DatabaseThread::instance(), parent)
{}

QMLPurchaseTransactionItemModel::QMLPurchaseTransactionItemModel(DatabaseThread &thread, QObject *parent) :
    AbstractTransactionItemModel(thread, parent)
{}

int QMLPurchaseTransactionItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_products.count();
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
        return m_products.at(index.row()).id;
    case CategoryIdRole:
        return m_products.at(index.row()).category.id;
    case CategoryRole:
        return m_products.at(index.row()).category.category;
    case ProductIdRole:
        return m_products.at(index.row()).product.id;
    case ProductRole:
        return m_products.at(index.row()).product.product;
    case UnitPriceRole:
        return m_products.at(index.row()).product.monies.unitPrice.toDouble();
    case QuantityRole:
        return m_products.at(index.row()).product.quantity;
    case UnitIdRole:
        return m_products.at(index.row()).product.unit.id;
    case UnitRole:
        return m_products.at(index.row()).product.unit.unit;
    case CostRole:
        return m_products.at(index.row()).cost;
    case DiscountRole:
        return m_products.at(index.row()).discount;
    case NoteIdRole:
        return m_products.at(index.row()).note.id;
    case NoteRole:
        return m_products.at(index.row()).note.note;
    case SuspendedRole:
        return m_products.at(index.row()).flags.testFlag(Utility::RecordGroup::Suspended);
    case ArchivedRole:
        return m_products.at(index.row()).flags.testFlag(Utility::RecordGroup::Archived);
    case CreatedRole:
        return m_products.at(index.row()).created;
    case LastEditedRole:
        return m_products.at(index.row()).lastEdited;
    case UserIdRole:
        return m_products.at(index.row()).user.id;
    case UserRole:
        return m_products.at(index.row()).user.user;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLPurchaseTransactionItemModel::roleNames() const
{
    return {
        { TransactionItemIdRole, "transaction_item_id" },
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

void QMLPurchaseTransactionItemModel::tryQuery()
{
    if (transactionId() <= 0)
        return;

    setBusy(true);
    emit execute(new PurchaseQuery::ViewPurchasedProducts(transactionId(),
                                                          this));
}

void QMLPurchaseTransactionItemModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == PurchaseQuery::ViewPurchasedProducts::COMMAND) {
            beginResetModel();
            m_products = Utility::PurchasedProductList{ result.outcome().toMap().value("products").toList() };
            endResetModel();

            emit success();
        }
    } else {
        emit error();
    }
}

QString QMLPurchaseTransactionItemModel::columnName(int column) const
{
    switch (column) {
    case CategoryColumn:
        return QStringLiteral("category");
    case ProductColumn:
        return QStringLiteral("product");
    case QuantityColumn:
        return QStringLiteral("quantity");
    case UnitPriceColumn:
        return QStringLiteral("unit_price");
    case CostColumn:
        return QStringLiteral("cost");
    }

    return QString();
}

void QMLPurchaseTransactionItemModel::removeSoldProduct(int row)
{
    setBusy(true);
    Utility::PurchasedProduct &product{ m_products[row] };
    product.transaction.id = transactionId();
    product.row = row;
    emit execute(new PurchaseQuery::RemovePurchasedProduct(product,
                                                           this));
}

