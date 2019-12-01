#ifndef SALESQLMANAGER_H
#define SALESQLMANAGER_H

#include "abstractsqlmanager.h"

#include "user/userprofile.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"

class SaleSqlManager : public AbstractSqlManager
{
public:
    explicit SaleSqlManager(const QString &connectionName);

    QueryResult execute(const QueryRequest &request) override final;
private:
    void addSaleTransaction(const QueryRequest &request, QueryResult &result, TransactionMode mode = TransactionMode::UseSqlTransaction); // throws DatabaseException
    void updateSuspendedTransaction(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewSaleCart(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void undoAddSaleTransaction(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewSaleTransactions(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewSaleTransactionItems(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewSaleHome(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewSaleReport(const QueryRequest &request, QueryResult &result); // throws DatabaseException
};

#endif // SALESQLMANAGER_H
