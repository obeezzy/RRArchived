#include "qmlstockitemmodel.h"
#include "database/databasethread.h"

#include <QDateTime>

QMLStockItemModel::QMLStockItemModel(QObject *parent) :
    QMLStockItemModel(DatabaseThread::instance(), parent)
{

}

QMLStockItemModel::QMLStockItemModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualTableModel(thread, parent),
    m_categoryId(-1)
{
    connect(this, &QMLStockItemModel::categoryIdChanged, this, &QMLStockItemModel::tryQuery);
}

int QMLStockItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int QMLStockItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QVariant QMLStockItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryIdRole:
        return m_records.at(index.row()).toMap().value("category_id").toInt();
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
    case ItemIdRole:
        return m_records.at(index.row()).toMap().value("item_id").toString();
    case ItemRole:
        return m_records.at(index.row()).toMap().value("item").toString();
    case DescriptionRole:
        return m_records.at(index.row()).toMap().value("description").toString();
    case DivisibleRole:
        return m_records.at(index.row()).toMap().value("divisible").toBool();
    case ImageSourceRole:
        return m_records.at(index.row()).toMap().value("image_source").toString();
    case QuantityRole:
        return m_records.at(index.row()).toMap().value("quantity").toDouble();
    case UnitRole:
        return m_records.at(index.row()).toMap().value("unit").toString();
    case UnitIdRole:
        return m_records.at(index.row()).toMap().value("unit_id").toInt();
    case CostPriceRole:
        return m_records.at(index.row()).toMap().value("cost_price").toDouble();
    case RetailPriceRole:
        return m_records.at(index.row()).toMap().value("retail_price").toDouble();
    case CurrencyRole:
        return m_records.at(index.row()).toMap().value("currency").toString();
    case CreatedRole:
        return m_records.at(index.row()).toMap().value("created").toDateTime();
    case LastEditedRole:
        return m_records.at(index.row()).toMap().value("last_edited").toDateTime();
    case UserRole:
        return m_records.at(index.row()).toMap().value("user").toString();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLStockItemModel::roleNames() const
{
    return {
        { CategoryIdRole, "category_id" },
        { CategoryRole, "category" },
        { ItemIdRole, "item_id" },
        { ItemRole, "item" },
        { DescriptionRole, "description" },
        { DivisibleRole, "divisible" },
        { ImageSourceRole, "image_source" },
        { QuantityRole, "quantity" },
        { UnitRole, "unit" },
        { UnitIdRole, "unit_id" },
        { CostPriceRole, "cost_price" },
        { RetailPriceRole, "retail_price" },
        { CurrencyRole, "currency" },
        { CreatedRole, "created" },
        { LastEditedRole, "last_edited" },
        { UserRole, "user" }
    };
}

QVariant QMLStockItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case ImageColumn:
                return tr("");
            case ItemColumn:
                return tr("Item");
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
            case ItemColumn:
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
            case ItemColumn:
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

int QMLStockItemModel::categoryId() const
{
    return m_categoryId;
}

void QMLStockItemModel::setCategoryId(int categoryId)
{
    if (m_categoryId == categoryId)
        return;

    m_categoryId = categoryId;
    emit categoryIdChanged();
}

void QMLStockItemModel::removeItem(int row)
{
    if (row < 0 || row >= rowCount())
        return;

    setBusy(true);

    QueryRequest request(this);
    const int itemId = index(row, 0).data(ItemIdRole).toInt();
    request.setCommand("remove_stock_item", {
                           { "item_id", itemId },
                           { "item_row", row },
                           { "item_info", m_records.at(row) },
                           { "can_undo", true }
                       }, QueryRequest::Stock);
    emit executeRequest(request);
}

void QMLStockItemModel::tryQuery()
{
    if (m_categoryId <= 0)
        return;

    setBusy(true);

    QueryRequest request(this);
    if (!filterText().trimmed().isEmpty() && sortColumn() > -1 && filterColumn() > -1) {
        request.setCommand("filter_stock_items", {
                               { "category_id", m_categoryId },
                               { "filter_text", filterText() },
                               { "filter_column", columnName(filterColumn()) },
                               { "sort_order", sortOrder() == Qt::AscendingOrder ? "ascending" : "descending" },
                               { "sort_column", columnName(sortColumn()) }
                           } , QueryRequest::Stock);

    } else {
        request.setCommand("view_stock_items", {
                               { "category_id", m_categoryId },
                               { "sort_order", sortOrder() == Qt::AscendingOrder ? "ascending" : "descending"} },
                           QueryRequest::Stock);
    }

    emit executeRequest(request);

}

void QMLStockItemModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "view_stock_items"
                || result.request().command() == "filter_stock_items") {
            beginResetModel();
            m_records = result.outcome().toMap().value("items").toList();
            endResetModel();

            emit success(ViewStockItemsSuccess);
        } else if (result.request().command() == "remove_stock_item") {
            const int row = result.request().params().value("item_row").toInt();
            removeItemFromModel(row);
            emit success(RemoveItemSuccess);
        } else if (result.request().command() == "undo_remove_stock_item") {
            const int row = result.request().params().value("item_row").toInt();
            const QVariantMap &itemInfo = result.request().params().value("item_info").toMap();
            undoRemoveItemFromModel(row, itemInfo);
            emit success(UndoRemoveItemSuccess);
        }
    } else {
        emit error();
    }
}

QString QMLStockItemModel::columnName(int column) const
{
    switch (column) {
    case ImageColumn:
        return "image";
    case ItemColumn:
        return "item";
    case QuantityColumn:
        return "quantity";
    case CostPriceColumn:
        return "cost_price";
    case ActionColumn:
        return "action";
    }

    return QString();
}

void QMLStockItemModel::filter()
{
    tryQuery();
}

void QMLStockItemModel::refresh()
{
    tryQuery();
}

void QMLStockItemModel::removeItemFromModel(int row)
{
    if (row < 0 || row >= rowCount())
        return;

    const int itemId = index(row, 0).data(ItemIdRole).toInt();
    emit itemRemoved(itemId);
    beginRemoveRows(QModelIndex(), row, row);
    m_records.removeAt(row);
    endRemoveRows();
}

void QMLStockItemModel::undoRemoveItemFromModel(int row, const QVariantMap &itemInfo)
{
    if (row < 0 || row >= rowCount() || itemInfo.isEmpty())
        return;

    beginInsertRows(QModelIndex(), row, row);
    m_records.insert(row, itemInfo);
    endInsertRows();
}
