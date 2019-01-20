#ifndef PURCHASESQLMANAGER_H
#define PURCHASESQLMANAGER_H

#include "abstractsqlmanager.h"

class PurchaseSqlManager : public AbstractSqlManager
{
public:
    explicit PurchaseSqlManager(const QString &connectionName);

    QueryResult execute(const QueryRequest &request) override final;
private:
    void viewPurchaseHome(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void addPurchaseTransaction(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void updateSuspendedTransaction(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewPurchaseCart(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void undoAddPurchaseTransaction(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewPurchaseTransactions(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewPurchaseTransactionItems(const QueryRequest &request, QueryResult &result); // throws DatabaseException
};

#endif // PURCHASESQLMANAGER_H
