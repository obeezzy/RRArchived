#include "qmlproductcategorymodel.h"
#include <QDebug>
#include "database/databasethread.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "query/stock/filtercategoriesbyproduct.h"
#include "query/stock/filterproductcategories.h"
#include "query/stock/removeproduct.h"
#include "query/stock/viewproductcategories.h"
#include "utility/stock/product.h"

QMLProductCategoryModel::QMLProductCategoryModel(QObject* parent)
    : QMLProductCategoryModel(DatabaseThread::instance(), parent)
{}

QMLProductCategoryModel::QMLProductCategoryModel(DatabaseThread& thread,
                                                 QObject* parent)
    : AbstractVisualListModel(thread, parent),
      m_productFilterCriteria{QStringLiteral("product"), QString()}
{
    connect(this, &QMLProductCategoryModel::productFilterTextChanged, this,
            &QMLProductCategoryModel::filter);
}

QString QMLProductCategoryModel::productFilterText() const
{
    return m_productFilterCriteria.text;
}

void QMLProductCategoryModel::setProductFilterText(
    const QString& productFilterText)
{
    if (m_productFilterCriteria.text == productFilterText)
        return;

    m_productFilterCriteria.text = productFilterText;
    emit productFilterTextChanged();
}

int QMLProductCategoryModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_categories.count();
}

QVariant QMLProductCategoryModel::data(const QModelIndex& index, int role) const
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

QHash<int, QByteArray> QMLProductCategoryModel::roleNames() const
{
    return {{ProductCategoryIdRole, "product_category_id"},
            {CategoryRole, "category"}};
}

void QMLProductCategoryModel::unarchiveProduct(int productId)
{
    if (productId <= 0)
        return;

    setBusy(true);
    auto removeStockProduct = new Query::Stock::RemoveProduct(
        Utility::Stock::Product{productId}, this);
    removeStockProduct->undoOnNextExecution();
    emit execute(removeStockProduct);
}

void QMLProductCategoryModel::updateCategory(
    const Utility::Stock::ProductCategory& category)
{
    if (category.id <= 0)
        return;

    if (sortOrder() == Qt::AscendingOrder) {
        if (category.row < 0) {
            for (int i = 0; i < rowCount(); ++i) {
                bool shouldBreak = false;
                const QString& categoryName = category.category;
                const QString& categoryNameInModel =
                    index(i).data(CategoryRole).toString();
                const QString& nextCategoryNameInModel =
                    index(i + 1).data(CategoryRole).toString();

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

int QMLProductCategoryModel::rowFromCategoryId(int categoryId) const
{
    if (categoryId > 0) {
        for (int i = 0; i < rowCount(); ++i) {
            const int categoryIdInModel =
                index(i).data(ProductCategoryIdRole).toInt();
            if (categoryId == categoryIdInModel)
                return i;
        }
    }

    return -1;
}

void QMLProductCategoryModel::removeCategory(int row)
{
    removeCategoryFromModel(row);
}

void QMLProductCategoryModel::removeCategoryFromModel(int row)
{
    if (row < 0 || row >= rowCount())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_categories.removeAt(row);
    endRemoveRows();
}

void QMLProductCategoryModel::undoRemoveCategoryFromModel(
    int row, const QVariantMap& category)
{
    if (row < 0 || row > rowCount())
        return;

    const auto categoryToReinsert = Utility::Stock::ProductCategory{category};
    beginInsertRows(QModelIndex(), row, row);
    m_categories.insert(row, categoryToReinsert);
    endInsertRows();
}

void QMLProductCategoryModel::tryQuery()
{
    setBusy(true);

    if (!m_productFilterCriteria.text.trimmed().isEmpty()) {
        emit execute(new Query::Stock::FilterCategoriesByProduct(
            m_productFilterCriteria, sortCriteria(), Utility::RecordGroup::None,
            this));
    } else if (!filterText().trimmed().isEmpty()) {
        emit execute(new Query::Stock::FilterProductCategories(
            filterCriteria(), sortCriteria(), Utility::RecordGroup::None,
            this));
    } else {
        emit execute(new Query::Stock::ViewProductCategories(
            sortCriteria(), Utility::RecordGroup::None, this));
    }
}

bool QMLProductCategoryModel::canProcessResult(const QueryResult& result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLProductCategoryModel::processResult(const QueryResult& result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() ==
                Query::Stock::ViewProductCategories::COMMAND ||
            result.request().command() ==
                Query::Stock::FilterCategoriesByProduct::COMMAND ||
            result.request().command() ==
                Query::Stock::FilterProductCategories::COMMAND) {
            beginResetModel();
            m_categories = Utility::Stock::ProductCategoryList{
                result.outcome().toMap().value("categories").toList()};
            endResetModel();

            emit success(ModelResult{ViewCategoriesSuccess});
        } else if (result.request().command() ==
                   Query::Stock::RemoveProduct::UNDO_COMMAND) {
            const auto category =
                Utility::Stock::ProductCategory{result.outcome().toMap()};
            updateCategory(category);
            emit success(ModelResult{UnarchiveProductSuccess});
        } else {
            emit success();
        }
    } else {
        emit error();
    }
}

void QMLProductCategoryModel::filter()
{
    tryQuery();
}
