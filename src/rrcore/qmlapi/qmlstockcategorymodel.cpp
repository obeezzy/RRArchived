#include "qmlstockcategorymodel.h"

#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "queryexecutors/stock.h"
#include <QDebug>

QMLStockCategoryModel::QMLStockCategoryModel(QObject *parent) :
    QMLStockCategoryModel(DatabaseThread::instance(), parent)
{}

QMLStockCategoryModel::QMLStockCategoryModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{
    connect(this, &QMLStockCategoryModel::itemFilterTextChanged, this, &QMLStockCategoryModel::filter);
}

QString QMLStockCategoryModel::itemFilterText() const
{
    return m_itemFilterText;
}

void QMLStockCategoryModel::setItemFilterText(const QString &itemFilterText)
{
    if (m_itemFilterText == itemFilterText)
        return;

    m_itemFilterText = itemFilterText;
    emit itemFilterTextChanged();
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
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLStockCategoryModel::roleNames() const
{
    return {
        { CategoryIdRole, "category_id" },
        { CategoryRole, "category" }
    };
}

void QMLStockCategoryModel::unarchiveItem(int itemId)
{
    if (itemId <= 0)
        return;

    setBusy(true);
    StockQuery::RemoveStockItem *removeStockItem = new StockQuery::RemoveStockItem(itemId, this);
    removeStockItem->undoOnNextExecution();
    emit execute(removeStockItem);
}

void QMLStockCategoryModel::updateCategory(int categoryId, const QVariantMap &categoryInfo)
{
    if (categoryId <= 0)
        return;

    const int row = rowFromCategoryId(categoryId);
    if (sortOrder() == Qt::AscendingOrder) {
        if (row < 0) {
            for (int i = 0; i < rowCount(); ++i) {
                bool shouldBreak = false;
                const QString &category = categoryInfo.value("category").toString();
                const QString &categoryInModel = index(i).data(CategoryRole).toString();
                const QString &nextCategoryInModel = index(i + 1).data(CategoryRole).toString();

                if (category > categoryInModel) {
                    if (nextCategoryInModel.trimmed().isEmpty()) {
                        beginInsertRows(QModelIndex(), i + 1, i + 1);
                        m_records.insert(i + 1, categoryInfo);
                        endInsertRows();
                        shouldBreak = true;
                    } else if (category < nextCategoryInModel) {
                        beginInsertRows(QModelIndex(), i + 1, i + 1);
                        m_records.insert(i + 1, categoryInfo);
                        endInsertRows();
                        shouldBreak = true;
                    }

                    if (shouldBreak)
                        break;
                } else if (category < categoryInModel) {
                    if (nextCategoryInModel.trimmed().isEmpty()) {
                        beginInsertRows(QModelIndex(), i, i);
                        m_records.insert(i, categoryInfo);
                        endInsertRows();
                        shouldBreak = true;
                    } else if (category < nextCategoryInModel) {
                        beginInsertRows(QModelIndex(), i, i);
                        m_records.insert(i, categoryInfo);
                        endInsertRows();
                        shouldBreak = true;
                    }

                    if (shouldBreak)
                        break;
                }
            }
        } else {
            emit dataChanged(index(row), index(row));
        }
    } else {
        // TODO: Unhandled case!
        Q_ASSERT(sortOrder() == Qt::AscendingOrder);
    }
}

int QMLStockCategoryModel::rowFromCategoryId(int categoryId) const
{
    if (categoryId > 0) {
        for (int i = 0; i < rowCount(); ++i) {
            const int categoryIdInModel = index(i).data(CategoryIdRole).toInt();
            if (categoryId == categoryIdInModel)
                return i;
        }
    }

    return -1;
}

void QMLStockCategoryModel::removeCategory(int row)
{
    removeCategoryFromModel(row);
}

void QMLStockCategoryModel::removeCategoryFromModel(int row)
{
    if (row < 0 || row >= rowCount())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_records.removeAt(row);
    endRemoveRows();
}

void QMLStockCategoryModel::undoRemoveCategoryFromModel(int row, const QVariantMap &categoryInfo)
{
    if (row < 0 || row > rowCount())
        return;

    beginInsertRows(QModelIndex(), row, row);
    m_records.insert(row, categoryInfo);
    endInsertRows();
}

void QMLStockCategoryModel::tryQuery()
{
    setBusy(true);

    if (!m_itemFilterText.trimmed().isEmpty()) {
        emit execute(new StockQuery::FilterStockCategoriesByItem(m_itemFilterText,
                                                             sortOrder(),
                                                             false,
                                                             this));
    } else if (!filterText().trimmed().isEmpty()) {
        emit execute(new StockQuery::FilterStockCategories(filterText(),
                                                       sortOrder(),
                                                       false,
                                                       this));
    } else {
        emit execute(new StockQuery::ViewStockCategories(sortOrder(),
                                                     false,
                                                     this));
    }
}

void QMLStockCategoryModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == StockQuery::ViewStockCategories::COMMAND
                || result.request().command() == StockQuery::FilterStockCategoriesByItem::COMMAND
                || result.request().command() == StockQuery::FilterStockCategories::COMMAND) {
            beginResetModel();
            m_records = result.outcome().toMap().value("categories").toList();
            endResetModel();

            emit success(ViewStockCategoriesSuccess);
        } else if (result.request().command() == StockQuery::RemoveStockItem::UNDO_COMMAND) {
            const int categoryId = result.outcome().toMap().value("category_id").toInt();
            const QString &category = result.outcome().toMap().value("category").toString();
            updateCategory(categoryId, QVariantMap {
                               { "category_id", categoryId },
                               { "category", category }
                           });
            emit success(UnarchiveItemSuccess);
        } else {
            emit success();
        }
    } else {
        emit error();
    }
}

void QMLStockCategoryModel::filter()
{
    tryQuery();
}
