#ifndef QMLSTOCKCATEGORYITEMMODEL_H
#define QMLSTOCKCATEGORYITEMMODEL_H

#include <QObject>

#include "models/abstractvisuallistmodel.h"

class QueryRequest;
class QueryResult;
class StockItemModel;

class QMLStockCategoryItemModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(int totalItems READ totalItems NOTIFY totalItemsChanged)
public:
    explicit QMLStockCategoryItemModel(QObject *parent = nullptr);
    explicit QMLStockCategoryItemModel(DatabaseThread &thread);

    enum FilterColumn {
        CategoryColumn, ItemColumn
    }; Q_ENUM(FilterColumn)

    enum SuccessCode {
        UnknownSuccess,
                ViewItemsSuccess,
                RemoveItemSuccess,
                UndoRemoveItemSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError
    }; Q_ENUM(ErrorCode)

    enum Roles {
        CategoryIdRole = Qt::UserRole,
        CategoryRole,
        ItemModelRole
    };

    int totalItems() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
signals:
    void totalItemsChanged();
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
    void filter() override final;
public slots:
    void removeItem(int itemId);
private:
    QList<StockItemModel *> m_stockItemModels;
    QStringList m_categories;
    QVariantList m_itemGroups;
    QHash<int, QString> m_categoryIdToCategoryHash;
    int m_totalItems;

    void setTotalItems(int totalItems);
    void removeItemFromModel(int categoryId, int itemId);
    void undoRemoveItemFromModel(int categoryId, int itemId, const QVariantMap &itemInfo);

    int indexOfCategoryIdInItemGroup(int categoryId);
};

#endif // QMLSTOCKCATEGORYITEMMODEL_H
