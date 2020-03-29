#ifndef QMLSTOCKPRODUCTCATEGORYMODEL_H
#define QMLSTOCKPRODUCTCATEGORYMODEL_H

#include "models/abstractvisuallistmodel.h"
#include "utility/stock/stockproductcategory.h"

class QMLStockProductCategoryModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(QString productFilterText READ productFilterText WRITE setProductFilterText NOTIFY productFilterTextChanged)
public:
    explicit QMLStockProductCategoryModel(QObject *parent = nullptr);
    explicit QMLStockProductCategoryModel(DatabaseThread &thread, QObject *parent = nullptr);

    enum Roles {
        ProductCategoryIdRole = Qt::UserRole,
        CategoryRole
    };

    enum SuccessCode {
        ViewCategoriesSuccess,
        RemoveCategorySuccess,
        UndoRemoveCategorySuccess,
        UnarchiveProductSuccess
    }; Q_ENUM(SuccessCode)

    QString productFilterText() const;
    void setProductFilterText(const QString &productFilterText);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    Q_INVOKABLE void unarchiveProduct(int productId);
    Q_INVOKABLE void removeCategory(int row);
signals:
    void productFilterTextChanged();
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
    void filter() override final;
private:
    Utility::StockProductCategoryList m_categories;
    Utility::FilterCriteria m_productFilterCriteria;

    void removeCategoryFromModel(int row);
    void undoRemoveCategoryFromModel(int row,
                                     const QVariantMap &category);
    void updateCategory(const Utility::StockProductCategory &category);
    int rowFromCategoryId(int categoryId) const;
};

#endif // QMLSTOCKPRODUCTCATEGORYMODEL_H
