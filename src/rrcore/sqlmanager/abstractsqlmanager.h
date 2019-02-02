#ifndef ABSTRACTSQLMANAGER_H
#define ABSTRACTSQLMANAGER_H

#include <QString>
#include <QVariantMap>
#include <QSqlRecord>
#include <initializer_list>
#include "database/queryresult.h"
#include "database/queryrequest.h"

struct ProcedureArgument {
    enum class Type { In, Out, InOut };
    Type type;
    QString name;
    QVariant value;
};

class AbstractSqlManager
{
public:
    enum class TransactionMode {
        UseSqlTransaction,
        SkipSqlTransaction
    };
    explicit AbstractSqlManager(const QString &connectionName);
    virtual ~AbstractSqlManager();

    virtual QueryResult execute(const QueryRequest &request) = 0;
protected:
    QString connectionName() const { return m_connectionName; }
    QVariantMap recordToMap(const QSqlRecord &);
    QSqlRecord mapToRecord(const QVariantMap &);

    void enforceArguments(QStringList argumentsToEnforce, const QVariantMap &params); // throw DatabaseException
    QList<QSqlRecord> callProcedure(const QString &procedure, std::initializer_list<ProcedureArgument> arguments); // throw DatabaseException

    int addNote(const QString &note, const QString &tableName); // throw DatabaseException
    void updateNote(int noteId, const QString &note, const QString &tableName = QString()); // throw DatabaseException
private:
    QString m_connectionName;
};

#endif // ABSTRACTSQLMANAGER_H
