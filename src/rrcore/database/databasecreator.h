#ifndef DATABASECREATOR_H
#define DATABASECREATOR_H

#include <QLoggingCategory>
#include <QSqlDatabase>

class QString;

class DatabaseCreator
{
public:
    explicit DatabaseCreator(QSqlDatabase connection = QSqlDatabase());

    void executeSqlFile(const QString& fileName);
    bool start();

private:
    QSqlDatabase m_connection;

    void openConnection(const QString& databaseName);
    void createDatabase();
    void dropDatabase();
    void initDatabase();
    void createProcedures();
    void updateBusinessDetails();

    void executeStoredProcedures(const QString& fileName);
};

Q_DECLARE_LOGGING_CATEGORY(lcdatabasecreator);

#endif  // DATABASECREATOR_H
