#ifndef DATABASECREATOR_H
#define DATABASECREATOR_H

#include <QSqlDatabase>
class QString;

class DatabaseCreator
{
public:
    explicit DatabaseCreator(QSqlDatabase connection = QSqlDatabase());

    void executeSqlFile(const QString &fileName); // throws DatabaseException!
    bool start();
private:
    QSqlDatabase m_connection;

    void initDatabase(); // throws DatabaseException!
    void createProcedures(); // throws DatabaseException!

    void executeStoredProcedures(const QString &fileName); // throws DatabaseException!
};

#endif // DATABASECREATOR_H
