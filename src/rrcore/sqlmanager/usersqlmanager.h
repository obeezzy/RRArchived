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
    bool storeProfile(QueryResult &result, const QString &userName, const QString &password); // throws DatabaseException

    void signInUser(const QueryRequest &, QueryResult &); // throws DatabaseException!
    void signUpUser(const QueryRequest &); // throws DatabaseException!
    void signUpRootUser(const QueryRequest &); // throws DatabaseException!
    void removeUser(const QueryRequest &); // throws DatabaseException!
    void viewUsers(const QueryRequest &, QueryResult &result); // throws DatabaseException!
    void viewUserPrivileges(const QueryRequest &, QueryResult &result); // throws DatabaseException
    void addUser(const QueryRequest &); // throws DatabaseException
    void activateUser(const QueryRequest &); // throws DatabaseException
    void updateUserPrivileges(const QueryRequest &); // throws DatabaseException
    void viewUserDetails(const QueryRequest &, QueryResult &); // throws DatabaseException
    void changePassword(const QueryRequest &); // throws DatabaseException
    void signOut(const QueryRequest &); // throws DatabaseException

    void grantPrivilege(const QString &privilege, const QString &userName, QSqlQuery &q); // throws DatabaseException!
    void createRRUser(const QString &userName, QSqlQuery &q); // throws DatabaseException!

    QString determineRank(const QVariantList &privileges);
};

#endif // USERSQLMANAGER_H
