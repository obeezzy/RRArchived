#ifndef EXPENSESQLMANAGER_H
#define EXPENSESQLMANAGER_H

#include "abstractsqlmanager.h"

class ExpenseSqlManager : public AbstractSqlManager
{
public:
    explicit ExpenseSqlManager(const QString &connectionName);

    QueryResult execute(const QueryRequest &request) override;
private:
    void addNewExpenseTransaction(const QueryRequest &request);
};

#endif // EXPENSESQLMANAGER_H
