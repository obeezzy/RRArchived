#ifndef EXPENSESQLMANAGER_H
#define EXPENSESQLMANAGER_H

#include "abstractsqlmanager.h"

class ExpenseSqlManager : public AbstractSqlManager
{
public:
    explicit ExpenseSqlManager(const QString &connectionName);

    QueryResult execute(const QueryRequest &request) override;
private:
    void addNewExpenseTransaction(const QueryRequest &request); // throws DatabaseException
    void viewExpenseTransactions(const QueryRequest &request, QueryResult &result); // throws DatabaseException
    void viewExpenseReport(const QueryRequest &request, QueryResult &result); // throws DatabaseException
};

#endif // EXPENSESQLMANAGER_H
