#ifndef USERSQLMANAGER_H
#define USERSQLMANAGER_H

#include "abstractsqlmanager.h"

class QSqlQuery;

class UserSqlManager : public AbstractSqlManager
{
public:
    UserSqlManager(const QString &connectionName);

    QueryResult execute(const QueryRequest &request) override final;
private:
    bool storeProfile(QueryResult &result);

    void signInUser(const QueryRequest &, QueryResult); // throws DatabaseException!
    void signUpUser(const QueryRequest &); // throws DatabaseException!
    void signUpRootUser(const QueryRequest &); // throws DatabaseException!
    void removeUser(const QueryRequest &); // throws DatabaseException!

    void grantPrivilege(const QString &privilege, const QString &userName, QSqlQuery &q); // throws DatabaseException!
    void createRRUser(const QString &userName, QSqlQuery &q); // throws DatabaseException!
};

#endif // USERSQLMANAGER_H
