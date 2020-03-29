#include "qmlsaletransactionitemmodel.h"
#include "database/databasethread.h"
#include "queryexecutors/sales.h"
#include "utility/saleutils.h"
#include <QDateTime>
#include <QDebug>

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

    return m_products.count();
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
    case CategoryIdRole:
        return m_products.at(index.row()).product.category.id;
    case CategoryRole:
        return m_products.at(index.row()).product.category.category;
    case ProductIdRole:
        return m_products.at(index.row()).product.id;
    case ProductRole:
        return m_products.at(index.row()).product.product;
    case UnitPriceRole:
        return m_products.at(index.row()).monies.unitPrice.toDouble();
    case QuantityRole:
        return m_products.at(index.row()).quantity.toDouble();
    case UnitIdRole:
        return m_products.at(index.row()).product.unit.id;
    case UnitRole:
        return m_products.at(index.row()).product.unit.unit;
    case CostRole:
        return m_products.at(index.row()).monies.cost.toDouble();
    case DiscountRole:
        return m_products.at(index.row()).monies.discount.toDouble();
    case NoteIdRole:
        return m_products.at(index.row()).note.id;
    case NoteRole:
        return m_products.at(index.row()).note.note;
    case SuspendedRole:
        return m_products.at(index.row()).flags.testFlag(Utility::RecordGroup::Suspended);
    case ArchivedRole:
        return m_products.at(index.row()).flags.testFlag(Utility::RecordGroup::Archived);
    case CreatedRole:
        return m_products.at(index.row()).timestamp.created;
    case LastEditedRole:
        return m_products.at(index.row()).timestamp.lastEdited;
    case UserIdRole:
        return m_products.at(index.row()).user.id;
    case UserRole:
        return m_products.at(index.row()).user.user;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLSaleTransactionItemModel::roleNames() const
{
    return {
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
    if (transactionId() <= 0)
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
            m_products = Utility::SoldProductList{ result.outcome().toMap().value("products").toList() };
            endResetModel();

            emit success();
        } else {
            emit success();
        }
    } else {
        emit error();
    }
}

QString QMLSaleTransactionItemModel::columnName(int column) const
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
