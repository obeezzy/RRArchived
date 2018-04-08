#ifndef DATABASECREATOR_H
#define DATABASECREATOR_H

#include <QSqlDatabase>
class QString;

class DatabaseCreator
{
public:
    explicit DatabaseCreator(QSqlDatabase connection);

    static void runSqlOnTestDatabase(const QString &fileName); // throws DatabaseException!

    void createDatabase(const QString &databaseName = QString()); // throws DatabsaeException!
    void dropDatabase(const QString &databaseName = QString()); // throws DatabsaeException!

    void createTables(); // throws DatabaseException!
    void createRRUser(const QString &userName, bool active = false, bool pending = false); // throws DatabaseException!
private:
    QSqlDatabase m_connection;
};

#endif // DATABASECREATOR_H
