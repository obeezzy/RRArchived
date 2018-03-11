#ifndef USERSQLMANAGER_H
#define USERSQLMANAGER_H

#include "abstractsqlmanager.h"

class UserSqlManager : public AbstractSqlManager
{
public:
    UserSqlManager(QSqlDatabase connection);

    QueryResult execute(const QueryRequest &request) override final;
private:
    bool storeProfile(QueryResult &result);

    void signInUser(const QueryRequest &, QueryResult); // throws DatabaseException!
    void signUpUser(const QueryRequest &); // throws DatabaseException!
};

#endif // USERSQLMANAGER_H
