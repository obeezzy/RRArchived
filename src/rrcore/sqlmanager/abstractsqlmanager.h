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
    explicit AbstractSqlManager(const QString &connectionName);
    virtual ~AbstractSqlManager();

    virtual QueryResult execute(const QueryRequest &request) = 0;
protected:
    QString connectionName() const { return m_connectionName; }
    QVariantMap recordToMap(const QSqlRecord &);
    QSqlRecord mapToRecord(const QVariantMap &);

    void enforceArguments(QStringList argumentsToEnforce, const QVariantMap &params); // throw DatabaseException
private:
    QString m_connectionName;
};

#endif // ABSTRACTSQLMANAGER_H
