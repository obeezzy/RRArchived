#ifndef QUERYEXECUTOR_H
#define QUERYEXECUTOR_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QSqlRecord>
#include <initializer_list>
#include <QLoggingCategory>

#include "database/queryrequest.h"
#include "database/queryresult.h"

struct ProcedureArgument {
    enum class Type { In, Out, InOut };
    Type type;
    QString name;
    QVariant value;
};

class QueryExecutor : public QObject
{
    Q_OBJECT
public:
    enum class TransactionMode {
        UseSqlTransaction,
        SkipSqlTransaction
    };
    explicit QueryExecutor(QObject *parent = nullptr);
    explicit QueryExecutor(const QueryRequest &request);
    explicit QueryExecutor(const QString &command,
                           const QVariantMap &params,
                           QueryRequest::QueryGroup queryGroup,
                           QObject *receiver);
    QueryExecutor(const QueryExecutor &other);
    QueryExecutor &operator=(const QueryExecutor &other);
    virtual ~QueryExecutor();

    QueryRequest request() const;
    QueryRequest &request();
    virtual QueryResult execute();

    bool isValid() const;
    bool canUndo() const;
    bool isUndoSet() const;
    void undoOnNextExecution(bool undo = true);

    QString connectionName() const;
    void setConnectionName(const QString &connectionName);

    friend QDebug operator<<(QDebug debug, const QueryExecutor &queryExecutor)
    {
        debug.nospace() << "QueryExecutor("
                        << queryExecutor.request()
                        << ")";

        return debug;
    }
protected:
    QVariantMap recordToMap(const QSqlRecord &);
    QSqlRecord mapToRecord(const QVariantMap &);

    void enforceArguments(QStringList argumentsToEnforce, const QVariantMap &params); // throw DatabaseException
    QList<QSqlRecord> callProcedure(const QString &procedure, std::initializer_list<ProcedureArgument> arguments); // throw DatabaseException

    int addNote(const QString &note, const QString &tableName); // throw DatabaseException
    void updateNote(int noteId, const QString &note, const QString &tableName = QString()); // throw DatabaseException
private:
    QueryRequest m_request;
    QString m_connectionName;
};

Q_DECLARE_LOGGING_CATEGORY(queryExecutor);

#endif // QUERYEXECUTOR_H
