#ifndef STOCKSQLMANAGER_H
#define STOCKSQLMANAGER_H

#include "sqlmanager/abstractsqlmanager.h"

class StockSqlManager : public AbstractSqlManager
{
public:
    explicit StockSqlManager(const QString &connectionName);

    QueryResult execute(const QueryRequest &request) override final;
private:
    void addNewStockItem(const QueryRequest &request); // NOTE: throws DatabaseException!
    void moveItemToStockCategory(const QueryRequest &request); // NOTE: throws DatabaseException!
    void updateStockItem(const QueryRequest &request); // NOTE: throws DatabaseException!
    void viewStockItems(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!
    void viewStockItemCount(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!
    void viewStockItemDetails(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!
    void viewStockCategories(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!
    void removeStockItem(const QueryRequest &request); // NOTE: throws DatabaseException!
    void undoRemoveStockItem(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!
    void viewStockReport(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!

    void filterStockCategories(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!
    void filterStockCategoriesByItem(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!
    void filterStockItems(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!
    void filterStockItemCount(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!
};

#endif // STOCKSQLMANAGER_H
