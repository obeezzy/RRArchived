#ifndef DEBTORSQLMANAGER_H
#define DEBTORSQLMANAGER_H

#include "sqlmanager/abstractsqlmanager.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include <QSqlDatabase>

class DebtorSqlManager : public AbstractSqlManager
{
public:
    explicit DebtorSqlManager(const QString &connectionName);

    QueryResult execute(const QueryRequest &request) override final;
private:
    void addNewDebtor(const QueryRequest &request, QueryResult &result); // throws DatabaseException!
    void undoAddNewDebtor(const QueryRequest &request, QueryResult &result); // throws DatabaseException!
    void updateDebtor(const QueryRequest &request, QueryResult &result); // throws DatabaseException!
    void viewDebtors(const QueryRequest &request, QueryResult &result); // throws DatabaseException!
    void removeDebtor(const QueryRequest &request, QueryResult &result); // throws DatabaseException!
    void undoRemoveDebtor(const QueryRequest &request, QueryResult &result); // throws DatabaseException!
    void viewDebtTransactions(const QueryRequest &request, QueryResult &result); // throws DatabaseException!
    void viewDebtorDetails(const QueryRequest &request, QueryResult &result); // throws DatabaseException!
};

#endif // DEBTORSQLMANAGER_H
