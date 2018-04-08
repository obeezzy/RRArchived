#include "qmlstockcategoryitemmodel.h"
#include <QObject>

#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "models/stockitemmodel.h"

QMLStockCategoryItemModel::QMLStockCategoryItemModel(QObject *parent)
    : AbstractVisualListModel(parent)
{
}

int QMLStockCategoryItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_categoryRecords.count();
}

QVariant QMLStockCategoryItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryIdRole: {
        if (m_stockItemModels.isEmpty()) {
            return QVariant();
        } else {
            StockItemModel *model = m_stockItemModels.at(index.row());
            return model->data(model->index(0), StockItemModel::CategoryIdRole).toInt();
        }
    }
        break;
    case CategoryRole:
        return m_categoryRecords.keys().at(index.row());
        break;
    case ItemModelRole:
        return QVariant::fromValue<QObject *>(m_stockItemModels.at(index.row()));
        break;
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

void QMLStockCategoryItemModel::processResult(const QueryResult &result)
{
    if (this != result.request().parent())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "view_stock_items") {
            beginResetModel();

            qDeleteAll(m_stockItemModels);
            m_stockItemModels.clear();
            m_categoryRecords = result.outcome().toMap().value("categories").toMap();

            QMapIterator<QString, QVariant> categoryIter(m_categoryRecords);
            int row = 0;
            while (categoryIter.hasNext()) {
                auto pair = categoryIter.next();
                const QVariantList items = pair.value().toList();

                StockItemModel *model = new StockItemModel(this);
                model->setCategory(pair.key());
                if (!items.isEmpty())
                    model->setCategoryId(items.first().toMap().value("category_id").toInt());

                m_categoryIdToCategoryHash.insert(model->categoryId(), model->category());

                model->setItems(items);
                m_stockItemModels.append(model);

                ++row;
            }

            endResetModel();

            emit success(ItemsFetched);
        } else if (result.request().command() == "remove_stock_item" && !result.request().params().value("undo").toBool()) {
            const int categoryId = result.outcome().toMap().value("category_id").toInt();
            const int itemId = result.outcome().toMap().value("item_id").toInt();

            removeItemFromModel(categoryId, itemId);

            emit success(ItemRemoved);
        } else if (result.request().command() == "undo_remove_stock_item") {
            const int categoryId = result.outcome().toMap().value("category_id").toInt();
            const int itemId = result.outcome().toMap().value("item_id").toInt();
            const QVariantMap itemInfo = result.outcome().toMap().value("item_info").toMap();

            undoRemoveItemFromModel(categoryId, itemId, itemInfo);

            emit success(UndoSuccessful);
        }
    } else {
        emit error();
    }
}

void QMLStockCategoryItemModel::filter()
{
    if (filterColumn() == -1)
        return;

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

void QMLStockCategoryItemModel::refresh()
{
    if (!isBusy())
        tryQuery();
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
                m_categoryRecords.remove(m_categoryRecords.keys().at(i));
                model->deleteLater();
                endRemoveRows();
            }
        }
    }
}

void QMLStockCategoryItemModel::undoRemoveItemFromModel(int categoryId, int itemId, const QVariantMap &itemInfo)
{
    if (categoryId <= 0 || itemId <= 0)
        return;

    if (m_categoryRecords.contains(m_categoryIdToCategoryHash.value(categoryId))) {
        // Find model and append item

        for (int i = 0; i < m_stockItemModels.count(); ++i) {
            StockItemModel *model = m_stockItemModels.at(i);

            if (model->categoryId() == categoryId) {
                model->addItem(itemId, itemInfo);
                break;
            }
        }
    } else {
        // Append model
        StockItemModel *newModel = new StockItemModel(this);
        const QString category = m_categoryIdToCategoryHash.value(categoryId);
        newModel->setCategoryId(categoryId);
        newModel->setCategory(category);
        newModel->setItems( { { itemInfo } });

        for (int i = 0; i < m_stockItemModels.count(); ++i) {
            StockItemModel *model = m_stockItemModels.at(i);

            if (newModel->category().toLower() > model->category().toLower()) {
                beginInsertRows(QModelIndex(), i, i);
                m_categoryRecords.insert(newModel->category(), newModel->items());
                endInsertRows();

                break;
            }
        }
    }
}
