#ifndef SALESQLMANAGER_H
#define SALESQLMANAGER_H

#include "abstractsqlmanager.h"

#include "singletons/userprofile.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"

class SaleSqlManager : public AbstractSqlManager
{
public:
    explicit SaleSqlManager(QSqlDatabase connection);

    QueryResult execute(const QueryRequest &request) override final;
private:
    void addTransaction(const QueryRequest &, QueryResult &, bool skipSqlTransaction = false); // throws DatabaseException
    void updateSuspendedTransaction(QueryRequest request, QueryResult &); // throws DatabaseException
    void viewSaleCart(const QueryRequest &, QueryResult &result); // throws DatabaseException
    void undoAddTransaction(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewSaleTransactions(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewSaleItemsForTransaction(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewSaleHome(const QueryRequest &request, QueryResult &result); // throws DatabaseException
};

#endif // SALESQLMANAGER_H
