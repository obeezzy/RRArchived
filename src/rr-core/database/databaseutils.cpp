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
