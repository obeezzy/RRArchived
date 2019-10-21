#include "stockitemmodel.h"
#include <QDateTime>

StockItemModel::StockItemModel(QObject *parent) :
    AbstractVisualTableModel(parent),
    m_categoryId(-1)
{

}

int StockItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

int StockItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QVariant StockItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryIdRole:
        return m_records.at(index.row()).toMap().value("category_id").toInt();
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

QHash<int, QByteArray> StockItemModel::roleNames() const
{
    return {
        { CategoryIdRole, "category_id" },
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

QVariant StockItemModel::headerData(int section, Qt::Orientation orientation, int role) const
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
                return qMax(280.0, tableViewWidth() - 30 - 120 - 120 - 130 - 40);
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

QString StockItemModel::category() const
{
    return m_category;
}

void StockItemModel::setCategory(const QString &category)
{
    m_category = category;
}

int StockItemModel::categoryId() const
{
    return m_categoryId;
}

void StockItemModel::setCategoryId(int categoryId)
{
    m_categoryId = categoryId;
}

QVariantList StockItemModel::items() const
{
    return m_records;
}

void StockItemModel::setItems(const QVariantList &items)
{
    m_records = items;
}

bool StockItemModel::addItem(int itemId, const QVariantMap &itemInfo, Qt::SortOrder sortOrder)
{
    if (itemId <= 0 || itemInfo.isEmpty())
        return false;

    if (sortOrder == Qt::DescendingOrder) {
        qFatal("UNTESTED >> %s", Q_FUNC_INFO);
        for (int i = 0; i < rowCount(); ++i) {
            if (itemInfo.value("item").toString().toLower() < data(index(i, 0), ItemRole).toString().toLower() && sortOrder == Qt::DescendingOrder) {
                beginInsertRows(QModelIndex(), i, i);
                m_records.insert(i, itemInfo);
                endInsertRows();

                return true;
            }
        }
    } else {
        for (int i = rowCount() - 1; i >= -1; --i) {
            if (itemInfo.value("item").toString().toLower() > data(index(i, 0), ItemRole).toString().toLower()) {
                beginInsertRows(QModelIndex(), i + 1, i + 1);
                m_records.insert(i + 1, itemInfo);
                endInsertRows();

                return true;
            }
        }
    }

    return false;
}

bool StockItemModel::removeItem(int itemId)
{
    if (itemId <= 0)
        return false;

    for (int i = 0; i < rowCount(); ++i) {
        if (itemId == data(index(i, 0), ItemIdRole).toInt()) {
            beginRemoveRows(QModelIndex(), i, i);
            m_records.removeAt(i);
            endRemoveRows();

            return true;
        }
    }

    return false;
}

void StockItemModel::tryQuery()
{

}

void StockItemModel::processResult(const QueryResult result)
{
    Q_UNUSED(result)
}
