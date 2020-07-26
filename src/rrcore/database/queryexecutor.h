#ifndef QUERYEXECUTOR_H
#define QUERYEXECUTOR_H

#include <QLoggingCategory>
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <initializer_list>
#include "database/queryrequest.h"
#include "database/queryresult.h"

class QSqlRecord;
class QSqlQuery;

struct ProcedureArgument
{
    enum class Type
    {
        In,
        Out,
        InOut
    };
    Type type{Type::In};
    QString name;
    QVariant value;
};

class QueryExecutor : public QObject
{
    Q_OBJECT
public:
    enum class TransactionMode
    {
        UseSqlTransaction,
        SkipSqlTransaction
    };
    Q_ENUM(TransactionMode)

    enum class ExceptionPolicy
    {
        AllowExceptions,
        DisallowExceptions
    };
    Q_ENUM(ExceptionPolicy)

    explicit QueryExecutor(QObject* parent = nullptr);
    explicit QueryExecutor(const QueryRequest& request);
    explicit QueryExecutor(const QString& command, const QVariantMap& params,
                           QueryRequest::QueryGroup queryGroup,
                           QObject* receiver);
    QueryExecutor(const QueryExecutor& other);
    QueryExecutor& operator=(const QueryExecutor& other);
    virtual ~QueryExecutor();

    QueryRequest request() const;
    QueryRequest& request();
    virtual QueryResult execute();

    bool isValid() const;
    bool canUndo() const;
    bool isUndoSet() const;
    void undoOnNextExecution(bool undo = true);

    QString connectionName() const;
    void setConnectionName(const QString& connectionName);

    friend QDebug operator<<(QDebug debug, const QueryExecutor& queryExecutor)
    {
        debug.nospace() << "QueryExecutor(" << queryExecutor.request() << ")";

        return debug;
    }

protected:
    QVariantMap recordToMap(const QSqlRecord&) noexcept;
    QSqlRecord mapToRecord(const QVariantMap&) noexcept;

    void enforceArguments(QStringList argumentsToEnforce,
                          const QVariantMap& params);
    QList<QSqlRecord> callProcedure(
        const QString& procedure,
        std::initializer_list<ProcedureArgument> arguments);

    int addNote(const QString& note, const QString& tableName,
                ExceptionPolicy policy = ExceptionPolicy::AllowExceptions);
    void updateNote(int noteId, const QString& note,
                    const QString& tableName = QString(),
                    ExceptionPolicy policy = ExceptionPolicy::AllowExceptions);
    int addOrUpdateNote(
        int noteId, const QString& note, const QString& tableName,
        ExceptionPolicy policy = ExceptionPolicy::AllowExceptions);

    static QByteArray imageUrlToByteArray(
        const QUrl& imageUrl, qint64 maxSize = 2 * 1024 * 1000 /* 2MB limit */);

    static QUrl byteArrayToImageUrl(const QByteArray& imageData);

    static void beginTransaction(QSqlQuery& q);
    static void commitTransaction(QSqlQuery& q);
    static void rollbackTransaction(QSqlQuery& q);

private:
    QueryRequest m_request;
    QString m_connectionName;

    static QString generateFileName(const QByteArray& imageData);
};

Q_DECLARE_LOGGING_CATEGORY(lcqueryexecutor);

#endif  // QUERYEXECUTOR_H
