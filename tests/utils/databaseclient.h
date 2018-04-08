#ifndef DATABASECLIENT_H
#define DATABASECLIENT_H

#include <QSqlDatabase>

class DatabaseClient
{
public:
    explicit DatabaseClient();
    ~DatabaseClient();

    bool initialize();
    bool connectToTestDatabase();
    bool disconnectFromTestDatabase();
    bool createTables();
    bool createUser();

    bool addDummyItems();

    bool executeSql(const QString &sql);

    int userId() const;
    QString userName() const;

    QSqlDatabase connection();
private:
    QSqlDatabase m_connection;
};

#endif // DATABASECLIENT_H
