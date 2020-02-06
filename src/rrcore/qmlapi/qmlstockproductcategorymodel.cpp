#include "qmlstockproductcategorymodel.h"

#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "queryexecutors/stock.h"
#include <QDebug>

QMLStockProductCategoryModel::QMLStockProductCategoryModel(QObject *parent) :
    QMLStockProductCategoryModel(DatabaseThread::instance(), parent)
{}

QMLStockProductCategoryModel::QMLStockProductCategoryModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{
    connect(this, &QMLStockProductCategoryModel::productFilterTextChanged,
            this, &QMLStockProductCategoryModel::filter);
}

QString QMLStockProductCategoryModel::productFilterText() const
{
    return m_productFilterText;
}

void QMLStockProductCategoryModel::setProductFilterText(const QString &productFilterText)
{
    if (m_productFilterText == productFilterText)
        return;

    m_productFilterText = productFilterText;
    emit productFilterTextChanged();
}


int QMLStockProductCategoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant QMLStockProductCategoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryIdRole:
        return m_records.at(index.row()).toMap().value("product_category_id").toInt();
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLStockProductCategoryModel::roleNames() const
{
    return {
        { CategoryIdRole, "category_id" },
        { CategoryRole, "category" }
    };
}

void QMLStockProductCategoryModel::unarchiveProduct(int productId)
{
    if (productId <= 0)
        return;

    setBusy(true);
    StockQuery::RemoveStockProduct *removeStockProduct = new StockQuery::RemoveStockProduct(productId, this);
    removeStockProduct->undoOnNextExecution();
    emit execute(removeStockProduct);
}

void QMLStockProductCategoryModel::updateCategory(int categoryId, const QVariantMap &categoryInfo)
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

int QMLStockProductCategoryModel::rowFromCategoryId(int categoryId) const
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

void QMLStockProductCategoryModel::removeCategory(int row)
{
    removeCategoryFromModel(row);
}

void QMLStockProductCategoryModel::removeCategoryFromModel(int row)
{
    if (row < 0 || row >= rowCount())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_records.removeAt(row);
    endRemoveRows();
}

void QMLStockProductCategoryModel::undoRemoveCategoryFromModel(int row, const QVariantMap &categoryInfo)
{
    if (row < 0 || row > rowCount())
        return;

    beginInsertRows(QModelIndex(), row, row);
    m_records.insert(row, categoryInfo);
    endInsertRows();
}

void QMLStockProductCategoryModel::tryQuery()
{
    setBusy(true);

    if (!m_productFilterText.trimmed().isEmpty()) {
        emit execute(new StockQuery::FilterStockProductCategoriesByProduct(m_productFilterText,
                                                                           sortOrder(),
                                                                           false,
                                                                           this));
    } else if (!filterText().trimmed().isEmpty()) {
        emit execute(new StockQuery::FilterStockCategories(filterText(),
                                                           sortOrder(),
                                                           false,
                                                           this));
    } else {
        emit execute(new StockQuery::ViewStockProductCategories(sortOrder(),
                                                                false,
                                                                this));
    }
}

void QMLStockProductCategoryModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == StockQuery::ViewStockProductCategories::COMMAND
                || result.request().command() == StockQuery::FilterStockProductCategoriesByProduct::COMMAND
                || result.request().command() == StockQuery::FilterStockCategories::COMMAND) {
            beginResetModel();
            m_records = result.outcome().toMap().value("categories").toList();
            endResetModel();

            emit success(ViewCategoriesSuccess);
        } else if (result.request().command() == StockQuery::RemoveStockProduct::UNDO_COMMAND) {
            const int categoryId = result.outcome().toMap().value("product_category_id").toInt();
            const QString &category = result.outcome().toMap().value("category").toString();
            updateCategory(categoryId, QVariantMap {
                               { "product_category_id", categoryId },
                               { "category", category }
                           });
            emit success(UnarchiveProductSuccess);
        } else {
            emit success();
        }
    } else {
        emit error();
    }
}

void QMLStockProductCategoryModel::filter()
{
    tryQuery();
}
