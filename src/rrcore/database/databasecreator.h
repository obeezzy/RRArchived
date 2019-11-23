#ifndef DATABASECREATOR_H
#define DATABASECREATOR_H

#include <QSqlDatabase>
#include <QLoggingCategory>

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

Q_DECLARE_LOGGING_CATEGORY(databaseCreator);

#endif // DATABASECREATOR_H
