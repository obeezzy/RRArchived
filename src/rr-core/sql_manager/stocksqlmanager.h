#ifndef STOCKSQLMANAGER_H
#define STOCKSQLMANAGER_H

#include "rr-core/sql_manager/abstractsqlmanager.h"

class StockSqlManager : public AbstractSqlManager
{
public:
    explicit StockSqlManager(QSqlDatabase connection);

    QueryResult execute(const QueryRequest &request) override final;
private:
    void addNewItem(const QueryRequest &request); // NOTE: throws DatabaseException!
    void viewStockItems(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!
    void viewStockCategories(const QueryRequest &request, QueryResult &result); // NOTE: throws DatabaseException!

    QByteArray imageToByteArray(const QString &imageSource);
    QString byteArrayToImage(const QByteArray &imageData);
};

#endif // STOCKSQLMANAGER_H
