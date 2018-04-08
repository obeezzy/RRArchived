#ifndef ABSTRACTSQLMANAGER_H
#define ABSTRACTSQLMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QVariantMap>
#include <QSqlRecord>
#include "database/queryresult.h"
#include "database/queryrequest.h"

class AbstractSqlManager
{
public:
    explicit AbstractSqlManager(QSqlDatabase connection);
    virtual ~AbstractSqlManager();

    AbstractSqlManager &operator() (QSqlDatabase connection) {
        m_connection = connection;
        return *this;
    }

    virtual QueryResult execute(const QueryRequest &request) = 0;
protected:
    QSqlDatabase &connection() { return m_connection; }
    QVariantMap recordToMap(const QSqlRecord &);
    QSqlRecord mapToRecord(const QVariantMap &);
private:
    QSqlDatabase m_connection;
};

#endif // ABSTRACTSQLMANAGER_H
