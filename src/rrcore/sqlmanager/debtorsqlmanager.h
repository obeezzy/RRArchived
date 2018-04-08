#ifndef DEBTORSQLMANAGER_H
#define DEBTORSQLMANAGER_H

#include "sqlmanager/abstractsqlmanager.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include <QSqlDatabase>

class DebtorSqlManager : public AbstractSqlManager
{
public:
    explicit DebtorSqlManager(QSqlDatabase connection);

    QueryResult execute(const QueryRequest &request) override final;
private:
    void viewDebtors(const QueryRequest &, QueryResult &); // throws DatabaseException
};

#endif // DEBTORSQLMANAGER_H
