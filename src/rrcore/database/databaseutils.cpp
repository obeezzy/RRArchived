#include "databaseutils.h"
#include <QSqlQuery>

DatabaseUtils::DatabaseUtils(QObject *parent)
    : QObject(parent)
{

}

bool DatabaseUtils::beginTransaction(QSqlQuery &q)
{
    if (!q.exec("SET AUTOCOMMIT = 0"))
        return false;

    return q.exec("START TRANSACTION");
}

bool DatabaseUtils::commitTransaction(QSqlQuery &q)
{
    return q.exec("COMMIT");
}

bool DatabaseUtils::rollbackTransaction(QSqlQuery &q)
{
    return q.exec("ROLLBACK");
}

QVariant DatabaseUtils::lastInsertId(QSqlQuery &q)
{
    if (q.exec("SELECT LAST_INSERT_ID()") && q.first())
        return q.value(0);

    return false;
}

bool DatabaseUtils::connectToDatabase(const QString &userName, const QString &password, const QString &databaseName)
{
    QSqlDatabase connection;

    if (!QSqlDatabase::contains())
        connection = QSqlDatabase::addDatabase("QMYSQL");
    else
        connection = QSqlDatabase::database();

    connection.setDatabaseName(databaseName);
    connection.setHostName("localhost");
    connection.setPort(3306);
    connection.setUserName(userName);
    connection.setPassword(password);
    connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    if (!connection.open())
        return false;

    return true;
}
