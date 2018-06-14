#include "qmlstockcategorymodel.h"

#include "database/queryrequest.h"
#include "database/queryresult.h"
#include <QDebug>

QMLStockCategoryModel::QMLStockCategoryModel(QObject *parent) :
    AbstractVisualListModel(parent)
{

}

int QMLStockCategoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant QMLStockCategoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryIdRole:
        return m_records.at(index.row()).toMap().value("category_id").toInt();
        break;
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLStockCategoryModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(CategoryIdRole, "category_id");
    roles.insert(CategoryRole, "category");

    return roles;
}

void QMLStockCategoryModel::tryQuery()
{
    setBusy(true);
    QueryRequest request(this);

    request.setCommand("view_stock_categories", QVariantMap(), QueryRequest::Stock);
    emit executeRequest(request);
}

void QMLStockCategoryModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        beginResetModel();
        m_records = result.outcome().toMap().value("categories").toList();
        endResetModel();

        emit success();
    } else {
        emit error();
    }
}
