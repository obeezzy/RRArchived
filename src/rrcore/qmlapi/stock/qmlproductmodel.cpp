#include "qmlproductmodel.h"
#include "database/databasethread.h"
#include "query/stock/filterproducts.h"
#include "query/stock/removeproduct.h"
#include "query/stock/viewproducts.h"
#include "utility/common/recordgroup.h"
#include "utility/common/filtercriteria.h"
#include "utility/common/sortcriteria.h"
#include <QDateTime>
#include <QDebug>

QMLProductModel::QMLProductModel(QObject *parent) :
    QMLProductModel(DatabaseThread::instance(), parent)
{

}

QMLProductModel::QMLProductModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualTableModel(thread, parent)
{
    connect(this, &QMLProductModel::categoryIdChanged,
            this, &QMLProductModel::tryQuery);
}

int QMLProductModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_products.count();
}

int QMLProductModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QVariant QMLProductModel::data(const QModelIndex &index, int role) const
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
        return m_products.at(index.row()).quantity.toDouble();
    case UnitRole:
        return m_products.at(index.row()).unit.unit;
    case UnitIdRole:
        return m_products.at(index.row()).unit.id;
    case CostPriceRole:
        return m_products.at(index.row()).monies.costPrice.toDouble();
    case RetailPriceRole:
        return m_products.at(index.row()).monies.retailPrice.toDouble();
    case CurrencyRole:
        return m_products.at(index.row()).monies.retailPrice.currency().displayName();
    case CreatedRole:
        return m_products.at(index.row()).timestamp.created;
    case LastEditedRole:
        return m_products.at(index.row()).timestamp.lastEdited;
    case UserRole:
        return m_products.at(index.row()).user.user;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLProductModel::roleNames() const
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
        { UnitRole, "product_unit" },
        { UnitIdRole, "product_unit_id" },
        { CostPriceRole, "cost_price" },
        { RetailPriceRole, "retail_price" },
        { CurrencyRole, "currency" },
        { CreatedRole, "created" },
        { LastEditedRole, "last_edited" },
        { UserRole, "user" }
    };
}

QVariant QMLProductModel::headerData(int section,
                                          Qt::Orientation orientation,
                                          int role) const
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

int QMLProductModel::categoryId() const
{
    return m_categoryId;
}

void QMLProductModel::setCategoryId(int categoryId)
{
    if (m_categoryId == categoryId)
        return;

    m_categoryId = categoryId;
    emit categoryIdChanged();
}

void QMLProductModel::removeProduct(int row)
{
    if (row < 0 || row >= rowCount())
        return;

    setBusy(true);
    Utility::Stock::Product product{ m_products[row] };
    product.row = row;
    emit execute(new Query::Stock::RemoveProduct(product,
                                                    this));
}

void QMLProductModel::tryQuery()
{
    if (m_categoryId <= 0)
        return;

    setBusy(true);
    if (filterCriteria().isValid() && sortCriteria().isValid()) {
        emit execute(new Query::Stock::FilterProducts(filterCriteria(),
                                                         sortCriteria(),
                                                         m_categoryId,
                                                         this));

    } else {
        emit execute(new Query::Stock::ViewProducts(m_categoryId,
                                                       sortCriteria(),
                                                       this));
    }
}

bool QMLProductModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLProductModel::processResult(const QueryResult &result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == Query::Stock::ViewProducts::COMMAND
                || result.request().command() == Query::Stock::FilterProducts::COMMAND) {
            beginResetModel();
            m_products = Utility::Stock::ProductList{ result.outcome().toMap().value("products").toList() };
            endResetModel();

            emit success(ModelResult{ ViewProductsSuccess });
        } else if (result.request().command() == Query::Stock::RemoveProduct::COMMAND) {
            const int row = result.request().params().value("row").toInt();
            removeProductFromModel(row);
            emit success(ModelResult{ RemoveProductSuccess });
        } else if (result.request().command() == Query::Stock::RemoveProduct::UNDO_COMMAND) {
            const auto product = Utility::Stock::Product{ result.request().params().value("product").toMap() };
            undoRemoveProductFromModel(product);
            emit success(ModelResult{ UndoRemoveProductSuccess });
        }
    } else {
        emit error();
    }
}

QString QMLProductModel::columnName(int column) const
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

void QMLProductModel::filter()
{
    tryQuery();
}

void QMLProductModel::refresh()
{
    tryQuery();
}

void QMLProductModel::undoLastCommit()
{
    QueryRequest request{ lastSuccessfulRequest() };
    QVariantMap params{ lastSuccessfulRequest().params() };
    if (lastSuccessfulRequest().command() == Query::Stock::RemoveProduct::COMMAND) {
        setBusy(true);
        auto query = new Query::Stock::RemoveProduct(request, this);
        query->undoOnNextExecution(true);
        emit execute(query);
    }
}

void QMLProductModel::removeProductFromModel(int row)
{
    if (row < 0 || row >= rowCount())
        return;

    const int productId = index(row, 0).data(ProductIdRole).toInt();
    emit productRemoved(productId);
    beginRemoveRows(QModelIndex(), row, row);
    Utility::Stock::Product product{ m_products.takeAt(row) };
    product.row = row;
    endRemoveRows();
}

void QMLProductModel::undoRemoveProductFromModel(const Utility::Stock::Product &product)
{
    if (product.row < 0 || product.row >= rowCount())
        return;

    const int row = product.row;
    beginInsertRows(QModelIndex(), row, row);
    m_products.insert(row, product);
    endInsertRows();
}
