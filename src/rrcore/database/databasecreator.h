#ifndef DATABASECREATOR_H
#define DATABASECREATOR_H

#include <QSqlDatabase>
class QString;

class DatabaseCreator
{
public:
    explicit DatabaseCreator();
    explicit DatabaseCreator(QSqlDatabase connection);

    void executeSqlFile(const QString &fileName); // throws DatabaseException!

    void createDatabase(const QString &databaseName = QString()); // throws DatabsaeException!
    void dropDatabase(const QString &databaseName = QString()); // throws DatabsaeException!

    void createTables(); // throws DatabaseException!
    void createRRUser(const QString &userName, bool active = false, bool pending = false); // throws DatabaseException!

    bool start();
private:
    QSqlDatabase m_connection;

    void initDatabase(); // throws DatabaseException!
    void createProcedures(); // throws DatabaseException!

    void executeStoredProcedures(const QString &fileName);
};

#endif // DATABASECREATOR_H
