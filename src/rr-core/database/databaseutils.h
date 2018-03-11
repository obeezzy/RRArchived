#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QObject>
#include <QVariant>

class QSqlQuery;

class DatabaseUtils : public QObject
{
    Q_OBJECT
public:
    static bool beginTransaction(QSqlQuery &q);
    static bool commitTransaction(QSqlQuery &q);
    static bool rollbackTransaction(QSqlQuery &q);
    static QVariant lastInsertId(QSqlQuery &q);
private:
    explicit DatabaseUtils(QObject *parent = nullptr);
};

#endif // DATABASEUTILS_H
