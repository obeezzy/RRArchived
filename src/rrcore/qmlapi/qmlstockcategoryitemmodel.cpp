#include "qmlstockcategoryitemmodel.h"
#include <QObject>

#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "models/stockitemmodel.h"

QMLStockCategoryItemModel::QMLStockCategoryItemModel(QObject *parent) :
    QMLStockCategoryItemModel(DatabaseThread::instance(), parent)
{}

QMLStockCategoryItemModel::QMLStockCategoryItemModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent),
    m_totalItems(0)
{
}

int QMLStockCategoryItemModel::totalItems() const
{
    return m_totalItems;
}

int QMLStockCategoryItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_itemGroups.count();
}

QVariant QMLStockCategoryItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryIdRole:
    {
        StockItemModel *model = m_stockItemModels.at(index.row());
        return model->data(model->index(0), StockItemModel::CategoryIdRole).toInt();
    }
    case CategoryRole:
        return m_categories.at(index.row());
    case ItemModelRole:
        return QVariant::fromValue<QObject *>(m_stockItemModels.at(index.row()));
    }

    return QVariant();
}

QHash<int, QByteArray> QMLStockCategoryItemModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(CategoryIdRole, "category_id");
    roles.insert(CategoryRole, "category");
    roles.insert(ItemModelRole, "item_model");

    return roles;
}

void QMLStockCategoryItemModel::tryQuery()
{
    setBusy(true);

    QueryRequest request(this);
    request.setCommand("view_stock_items", { { "sort_order", Qt::AscendingOrder } } , QueryRequest::Stock);

    emit executeRequest(request);
}

void QMLStockCategoryItemModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "view_stock_items") {
            beginResetModel();

            if (!m_stockItemModels.isEmpty()) {
                qDeleteAll(m_stockItemModels);
                m_stockItemModels.clear();
            }

            m_categories = result.outcome().toMap().value("categories").toStringList();
            m_itemGroups = result.outcome().toMap().value("item_groups").toList();

            for (const QVariant &itemGroup : m_itemGroups) {
                const QVariantList &items = itemGroup.toList();

                StockItemModel *model = new StockItemModel(this);
                if (!items.isEmpty()) {
                    model->setCategory(items.first().toMap().value("category").toString());
                    model->setCategoryId(items.first().toMap().value("category_id").toInt());
                    m_categoryIdToCategoryHash.insert(model->categoryId(), model->category());
                    model->setItems(items);
                    m_stockItemModels.append(model);
                }
            }

            endResetModel();

            setTotalItems(result.outcome().toMap().value("total_items").toInt());

            emit success(ViewItemsSuccess);
        } else if (result.request().command() == "remove_stock_item" && !result.request().params().value("undo").toBool()) {
            const int categoryId = result.outcome().toMap().value("category_id").toInt();
            const int itemId = result.outcome().toMap().value("item_id").toInt();

            removeItemFromModel(categoryId, itemId);

            emit success(RemoveItemSuccess);
        } else if (result.request().command() == "undo_remove_stock_item") {
            const int categoryId = result.outcome().toMap().value("category_id").toInt();
            const int itemId = result.outcome().toMap().value("item_id").toInt();
            const QVariantMap &itemInfo = result.outcome().toMap().value("item_info").toMap();

            undoRemoveItemFromModel(categoryId, itemId, itemInfo);

            emit success(UndoRemoveItemSuccess);
        }
    } else {
        emit error();
    }
}

void QMLStockCategoryItemModel::filter()
{
    if (filterColumn() == -1)
        return;

    setBusy(true);

    QueryRequest request(this);
    QVariantMap params;

    params.insert("filter_text", filterText());

    switch (filterColumn()) {
    case CategoryColumn:
        params.insert("filter_column", "category");
        break;
    default:
        params.insert("filter_column", "item");
        break;
    }

    request.setCommand("view_stock_items", params, QueryRequest::Stock);
    emit executeRequest(request);
}

void QMLStockCategoryItemModel::removeItem(int itemId)
{
    if (itemId <= 0)
        return;

    setBusy(true);

    QueryRequest request(this);
    request.setCommand("remove_stock_item", { { "item_id", itemId }, { "can_undo", true } }, QueryRequest::Stock);
    emit executeRequest(request);
}

void QMLStockCategoryItemModel::setTotalItems(int totalItems)
{
    if (m_totalItems == totalItems)
        return;

    m_totalItems = totalItems;
    emit totalItemsChanged();
}

void QMLStockCategoryItemModel::removeItemFromModel(int categoryId, int itemId)
{
    if (categoryId <= 0 || itemId <= 0)
        return;

    for (int i = 0; i < m_stockItemModels.count(); ++i) {
        StockItemModel *model = m_stockItemModels.at(i);

        if (model->categoryId() == categoryId) {
            model->removeItem(itemId);

            if (model->rowCount() == 0) {
                beginRemoveRows(QModelIndex(), i, i);
                m_categories.removeAt(i);
                m_itemGroups.removeAt(i);
                m_stockItemModels.removeAt(i);
                endRemoveRows();
            }

            setTotalItems(m_totalItems - 1);
            break;
        }
    }
}

void QMLStockCategoryItemModel::undoRemoveItemFromModel(int categoryId, int itemId, const QVariantMap &itemInfo)
{
    if (categoryId <= 0 || itemId <= 0 || itemInfo.isEmpty())
        return;

    // Find model and append item
    const int index = indexOfCategoryIdInItemGroup(categoryId);
    if (index > -1) {
        StockItemModel *model = m_stockItemModels.at(index);
        model->addItem(itemId, itemInfo);
        setTotalItems(m_totalItems + 1);
        // Append model
    } else {
        StockItemModel *newModel = new StockItemModel(this);
        const QString &category = m_categoryIdToCategoryHash.value(categoryId);
        newModel->setCategoryId(categoryId);
        newModel->setCategory(category);
        newModel->setItems({ { itemInfo } });

        for (int i = 0; i < m_stockItemModels.count(); ++i) {
            StockItemModel *model = m_stockItemModels.at(i);

            if (newModel->category().toLower() < model->category().toLower()) {
                beginInsertRows(QModelIndex(), i, i);
                m_categories.insert(i, newModel->category());
                m_itemGroups.insert(i, newModel->items());
                m_stockItemModels.insert(i, newModel);
                setTotalItems(m_totalItems + 1);
                endInsertRows();

                break;
            }
        }
    }
}

int QMLStockCategoryItemModel::indexOfCategoryIdInItemGroup(int categoryId)
{
    if (categoryId <= 0)
        return -1;

    int index = 0;
    for (const QVariant &itemGroup : m_itemGroups) {
        const QVariantList &items = itemGroup.toList();

        if (!items.isEmpty() && items.first().toMap().value("category_id").toInt() == categoryId)
            return index;

        ++index;
    }

    return -1;
}
