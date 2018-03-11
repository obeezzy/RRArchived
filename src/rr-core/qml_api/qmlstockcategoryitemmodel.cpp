#include "qmlstockcategoryitemmodel.h"
#include <QObject>

#include "rr-core/database/queryrequest.h"
#include "rr-core/database/queryresult.h"
#include "rr-core/database/databasethread.h"
#include "rr-core/models/stockitemmodel.h"

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

    request.setCommand("view_stock_items", { }, QueryRequest::Stock);

    emit executeRequest(request);
}

void QMLStockCategoryItemModel::processResult(const QueryResult &result)
{
    if (this != result.request().parent())
        return;

    setBusy(false);

    beginResetModel();

    qDeleteAll(m_stockItemModels);
    m_stockItemModels.clear();
    m_categoryRecords = result.outcome().toMap();

    if (result.isSuccessful()) {
        QMapIterator<QString, QVariant> categoryIter(m_categoryRecords);
        while (categoryIter.hasNext()) {
            auto pair = categoryIter.next();
            const QVariantList items = pair.value().toList();

            StockItemModel *model = new StockItemModel(this);
            model->setCategory(pair.key());
            model->setItems(items);
            m_stockItemModels.append(model);
        }

        emit success();
    } else {
        emit error(1);
    }

    endResetModel();
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
