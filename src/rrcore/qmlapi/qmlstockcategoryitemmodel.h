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
public:
    explicit QMLStockCategoryItemModel(QObject *parent = nullptr);

    enum Column {
        CategoryColumn, ItemColumn
    }; Q_ENUM(Column)

    enum SuccessCodes {
        ItemsFetched, ItemRemoved, UndoSuccessful
    }; Q_ENUM(SuccessCodes)

    enum Roles {
        CategoryIdRole = Qt::UserRole,
        CategoryRole,
        ItemModelRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
    void filter() override final;
public slots:
    void refresh();
    void removeItem(int itemId);
private:
    QList<StockItemModel *> m_stockItemModels;
    QVariantMap m_categoryRecords;
    QHash<int, QString> m_categoryIdToCategoryHash;

    void removeItemFromModel(int categoryId, int itemId);
    void undoRemoveItemFromModel(int categoryId, int itemId, const QVariantMap &itemInfo);
};

#endif // QMLSTOCKCATEGORYITEMMODEL_H
