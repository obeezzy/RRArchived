#include "qmlstockproductmodel.h"
#include "database/databasethread.h"
#include <QDateTime>
#include "queryexecutors/stock.h"

QMLStockProductModel::QMLStockProductModel(QObject *parent) :
    QMLStockProductModel(DatabaseThread::instance(), parent)
{

}

QMLStockProductModel::QMLStockProductModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualTableModel(thread, parent),
    m_categoryId(-1)
{
    connect(this, &QMLStockProductModel::categoryIdChanged,
            this, &QMLStockProductModel::tryQuery);
}

int QMLStockProductModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_products.count();
}

int QMLStockProductModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QVariant QMLStockProductModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case ProductIdRole:
        return m_products.at(index.row()).id;
    case CategoryIdRole:
        return m_products.at(index.row()).category.id;
    case CategoryRole:
        return m_products.at(index.row()).category.category;
    case ProductRole:
        return m_products.at(index.row()).product;
    case DescriptionRole:
        return m_products.at(index.row()).description;
    case DivisibleRole:
        return m_products.at(index.row()).flags.testFlag(Utility::RecordGroup::Divisible);
    case ImageUrlRole:
        return m_products.at(index.row()).imageUrl;
    case QuantityRole:
        return m_products.at(index.row()).quantity;
    case UnitRole:
        return m_products.at(index.row()).unit.unit;
    case UnitIdRole:
        return m_products.at(index.row()).unit.id;
    case CostPriceRole:
        return m_products.at(index.row()).costPrice;
    case RetailPriceRole:
        return m_products.at(index.row()).retailPrice;
    case CurrencyRole:
        return m_products.at(index.row()).currency;
    case CreatedRole:
        return m_products.at(index.row()).created;
    case LastEditedRole:
        return m_products.at(index.row()).lastEdited;
    case UserRole:
        return m_products.at(index.row()).user.user;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLStockProductModel::roleNames() const
{
    return {
        { CategoryIdRole, "product_category_id" },
        { CategoryRole, "category" },
        { ProductIdRole, "product_id" },
        { ProductRole, "product" },
        { DescriptionRole, "description" },
        { DivisibleRole, "divisible" },
        { ImageUrlRole, "image_url" },
        { QuantityRole, "quantity" },
        { UnitRole, "unit" },
        { UnitIdRole, "product_unit_id" },
        { CostPriceRole, "cost_price" },
        { RetailPriceRole, "retail_price" },
        { CurrencyRole, "currency" },
        { CreatedRole, "created" },
        { LastEditedRole, "last_edited" },
        { UserRole, "user" }
    };
}

QVariant QMLStockProductModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case ImageColumn:
                return tr("");
            case ProductColumn:
                return tr("Product");
            case QuantityColumn:
                return tr("Qty");
            case CostPriceColumn:
                return tr("Cost price");
            case ActionColumn:
                return tr("Action");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case ImageColumn:
                return Qt::AlignHCenter;
            case ProductColumn:
                return Qt::AlignLeft;
            case QuantityColumn:
                return Qt::AlignRight;
            case CostPriceColumn:
                return Qt::AlignRight;
            case ActionColumn:
                return Qt::AlignHCenter;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
            case ImageColumn:
                return 30;
            case ProductColumn:
                return qMax(280.0, tableViewWidth() - 30 - 120 - 120 - 130);
            case QuantityColumn:
                return 120;
            case CostPriceColumn:
                return 120;
            case ActionColumn:
                return 130;
            }
        }
    }

    return section + 1;
}

int QMLStockProductModel::categoryId() const
{
    return m_categoryId;
}

void QMLStockProductModel::setCategoryId(int categoryId)
{
    if (m_categoryId == categoryId)
        return;

    m_categoryId = categoryId;
    emit categoryIdChanged();
}

void QMLStockProductModel::removeProduct(int row)
{
    if (row < 0 || row >= rowCount())
        return;

    setBusy(true);
    Utility::StockProduct product{ m_products[row] };
    product.row = row;
    emit execute(new StockQuery::RemoveStockProduct(product,
                                                    this));
}

void QMLStockProductModel::tryQuery()
{
    if (m_categoryId <= 0)
        return;

    setBusy(true);
    if (!filterText().trimmed().isEmpty() && sortColumn() > -1 && filterColumn() > -1) {
        emit execute(new StockQuery::FilterStockProducts(Utility::FilterCriteria {
                                                             columnName(filterColumn()),
                                                             filterText(),
                                                         }, Utility::SortCriteria {
                                                             columnName(sortColumn()),
                                                             sortOrder()
                                                         },
                                                         m_categoryId,
                                                         this));

    } else {
        emit execute(new StockQuery::ViewStockProducts(m_categoryId,
                                                       Utility::SortCriteria{ sortOrder() },
                                                       this));
    }
}

void QMLStockProductModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == StockQuery::ViewStockProducts::COMMAND
                || result.request().command() == StockQuery::FilterStockProducts::COMMAND) {
            beginResetModel();
            m_products = Utility::StockProductList{ result.outcome().toMap().value("products").toList() };
            endResetModel();

            emit success(ViewProductsSuccess);
        } else if (result.request().command() == StockQuery::RemoveStockProduct::COMMAND) {
            const int row = result.request().params().value("product_row").toInt();
            removeProductFromModel(row);
            emit success(RemoveProductSuccess);
        } else if (result.request().command() == StockQuery::RemoveStockProduct::UNDO_COMMAND) {
            const int row = result.request().params().value("product_row").toInt();
            const Utility::StockProduct product{ result.request().params().value("product").toMap() };
            undoRemoveProductFromModel(row, product);
            emit success(UndoRemoveProductSuccess);
        }
    } else {
        emit error();
    }
}

QString QMLStockProductModel::columnName(int column) const
{
    switch (column) {
    case ImageColumn:
        return QStringLiteral("image");
    case ProductColumn:
        return QStringLiteral("product");
    case QuantityColumn:
        return QStringLiteral("quantity");
    case CostPriceColumn:
        return QStringLiteral("cost_price");
    case ActionColumn:
        return QStringLiteral("action");
    }

    return QString();
}

void QMLStockProductModel::filter()
{
    tryQuery();
}

void QMLStockProductModel::refresh()
{
    tryQuery();
}

void QMLStockProductModel::removeProductFromModel(int row)
{
    if (row < 0 || row >= rowCount())
        return;

    const int productId = index(row, 0).data(ProductIdRole).toInt();
    emit productRemoved(productId);
    beginRemoveRows(QModelIndex(), row, row);
    m_products.removeAt(row);
    endRemoveRows();
}

void QMLStockProductModel::undoRemoveProductFromModel(int row,
                                                      const Utility::StockProduct &product)
{
    if (row < 0 || row >= rowCount())
        return;

    beginInsertRows(QModelIndex(), row, row);
    m_products.insert(row, product);
    endInsertRows();
}
