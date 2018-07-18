#ifndef CLIENTSQLMANAGER_H
#define CLIENTSQLMANAGER_H

#include "abstractsqlmanager.h"

class ClientSqlManager : public AbstractSqlManager
{
public:
    explicit ClientSqlManager(QSqlDatabase connection);

    QueryResult execute(const QueryRequest &request) override;
private:
    void viewClients(const QueryRequest &request, QueryResult &result); // throws DatabaseException!
};

#endif // CLIENTSQLMANAGER_H
