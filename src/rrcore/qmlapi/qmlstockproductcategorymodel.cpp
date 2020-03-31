#include "qmlstockproductcategorymodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "queryexecutors/stock.h"
#include <QDebug>

QMLStockProductCategoryModel::QMLStockProductCategoryModel(QObject *parent) :
    QMLStockProductCategoryModel(DatabaseThread::instance(), parent)
{}

QMLStockProductCategoryModel::QMLStockProductCategoryModel(DatabaseThread &thread,
                                                           QObject *parent) :
    AbstractVisualListModel(thread, parent),
    m_productFilterCriteria{ QStringLiteral("product"), QString() }
{
    connect(this, &QMLStockProductCategoryModel::productFilterTextChanged,
            this, &QMLStockProductCategoryModel::filter);
}

QString QMLStockProductCategoryModel::productFilterText() const
{
    return m_productFilterCriteria.text;
}

void QMLStockProductCategoryModel::setProductFilterText(const QString &productFilterText)
{
    if (m_productFilterCriteria.text == productFilterText)
        return;

    m_productFilterCriteria.text = productFilterText;
    emit productFilterTextChanged();
}


int QMLStockProductCategoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_categories.count();
}

QVariant QMLStockProductCategoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case ProductCategoryIdRole:
        return m_categories.at(index.row()).id;
    case CategoryRole:
        return m_categories.at(index.row()).category;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLStockProductCategoryModel::roleNames() const
{
    return {
        { ProductCategoryIdRole, "product_category_id" },
        { CategoryRole, "category" }
    };
}

void QMLStockProductCategoryModel::unarchiveProduct(int productId)
{
    if (productId <= 0)
        return;

    setBusy(true);
    auto removeStockProduct = new StockQuery::RemoveStockProduct(
                Utility::StockProduct{ productId },
                this);
    removeStockProduct->undoOnNextExecution();
    emit execute(removeStockProduct);
}

void QMLStockProductCategoryModel::updateCategory(const Utility::StockProductCategory &category)
{
    if (category.id <= 0)
        return;

    if (sortOrder() == Qt::AscendingOrder) {
        if (category.row < 0) {
            for (int i = 0; i < rowCount(); ++i) {
                bool shouldBreak = false;
                const QString &categoryName = category.category;
                const QString &categoryNameInModel = index(i).data(CategoryRole).toString();
                const QString &nextCategoryNameInModel = index(i + 1).data(CategoryRole).toString();

                if (categoryName > categoryNameInModel) {
                    if (nextCategoryNameInModel.trimmed().isEmpty()) {
                        beginInsertRows(QModelIndex(), i + 1, i + 1);
                        m_categories.insert(i + 1, category);
                        endInsertRows();
                        shouldBreak = true;
                    } else if (categoryName < nextCategoryNameInModel) {
                        beginInsertRows(QModelIndex(), i + 1, i + 1);
                        m_categories.insert(i + 1, category);
                        endInsertRows();
                        shouldBreak = true;
                    }

                    if (shouldBreak)
                        break;
                } else if (categoryName < categoryNameInModel) {
                    if (nextCategoryNameInModel.trimmed().isEmpty()) {
                        beginInsertRows(QModelIndex(), i, i);
                        m_categories.insert(i, category);
                        endInsertRows();
                        shouldBreak = true;
                    } else if (categoryName < nextCategoryNameInModel) {
                        beginInsertRows(QModelIndex(), i, i);
                        m_categories.insert(i, category);
                        endInsertRows();
                        shouldBreak = true;
                    }

                    if (shouldBreak)
                        break;
                }
            }
        } else {
            emit dataChanged(index(category.row), index(category.row));
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
            const int categoryIdInModel = index(i).data(ProductCategoryIdRole).toInt();
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
    m_categories.removeAt(row);
    endRemoveRows();
}

void QMLStockProductCategoryModel::undoRemoveCategoryFromModel(int row,
                                                               const QVariantMap &category)
{
    if (row < 0 || row > rowCount())
        return;

    const Utility::StockProductCategory &categoryToReinsert{ category };
    beginInsertRows(QModelIndex(), row, row);
    m_categories.insert(row, categoryToReinsert);
    endInsertRows();
}

void QMLStockProductCategoryModel::tryQuery()
{
    setBusy(true);

    if (!m_productFilterCriteria.text.trimmed().isEmpty()) {
        emit execute(new StockQuery::FilterStockProductCategoriesByProduct(
                         m_productFilterCriteria,
                         sortCriteria(),
                         Utility::RecordGroup::None,
                         this));
    } else if (!filterText().trimmed().isEmpty()) {
        emit execute(new StockQuery::FilterStockCategories(filterCriteria(),
                                                           sortCriteria(),
                                                           Utility::RecordGroup::None,
                                                           this));
    } else {
        emit execute(new StockQuery::ViewStockProductCategories(sortCriteria(),
                                                                Utility::RecordGroup::None,
                                                                this));
    }
}

bool QMLStockProductCategoryModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLStockProductCategoryModel::processResult(const QueryResult &result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == StockQuery::ViewStockProductCategories::COMMAND
                || result.request().command() == StockQuery::FilterStockProductCategoriesByProduct::COMMAND
                || result.request().command() == StockQuery::FilterStockCategories::COMMAND) {
            beginResetModel();
            m_categories = Utility::StockProductCategoryList{ result.outcome().toMap().value("categories").toList() };
            endResetModel();

            emit success(ModelResult{ ViewCategoriesSuccess });
        } else if (result.request().command() == StockQuery::RemoveStockProduct::UNDO_COMMAND) {
            const Utility::StockProductCategory &category{ result.outcome().toMap() };
            updateCategory(category);
            emit success(ModelResult{ UnarchiveProductSuccess });
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
