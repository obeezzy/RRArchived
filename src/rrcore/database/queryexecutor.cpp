#include "queryexecutor.h"
#include <QBuffer>
#include <QCryptographicHash>
#include <QDateTime>
#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include "user/userprofile.h"

Q_LOGGING_CATEGORY(lcqueryexecutor, "rrcore.database.queryexecutor");

const int HASH_INPUT_LENGTH = 64;

QueryExecutor::QueryExecutor(QObject* parent) : QObject(nullptr)
{
    Q_UNUSED(parent)
    qRegisterMetaType<QueryExecutor>("QueryExecutor");
    qCDebug(lcqueryexecutor)
        << "QueryExecutor created:" << m_request.command() << this;
}

QueryExecutor::QueryExecutor(const QueryRequest& request)
    : QObject(nullptr),
      m_request(request)
{
    qRegisterMetaType<QueryExecutor>("QueryExecutor");
    qCDebug(lcqueryexecutor)
        << "QueryExecutor created:" << m_request.command() << this;
}

QueryExecutor::QueryExecutor(const QString& command, const QVariantMap& params,
                             QueryRequest::QueryGroup queryGroup,
                             QObject* receiver)
    : QObject(nullptr)
{
    m_request.setCommand(command, params, queryGroup);
    m_request.setReceiver(receiver);
    qCDebug(lcqueryexecutor)
        << "QueryExecutor created:" << m_request.command() << this;
}

QueryExecutor::QueryExecutor(const QueryExecutor& other)
    : QObject(nullptr),
      m_request(other.request())
{
    qRegisterMetaType<QueryExecutor>("QueryExecutor");
}

QueryExecutor& QueryExecutor::operator=(const QueryExecutor& other)
{
    m_request = other.request();
    return *this;
}

QueryExecutor::~QueryExecutor()
{
    qCDebug(lcqueryexecutor)
        << "QueryExecutor destroyed:" << m_request.command() << this;
}

QueryRequest QueryExecutor::request() const
{
    return m_request;
}

QueryRequest& QueryExecutor::request()
{
    return m_request;
}

QueryResult QueryExecutor::execute()
{
    return QueryResult();
}

bool QueryExecutor::isValid() const
{
    return !m_request.command().trimmed().isEmpty();
}

bool QueryExecutor::canUndo() const
{
    return request().canUndo();
}

bool QueryExecutor::isUndoSet() const
{
    return request().isUndoSet();
}

void QueryExecutor::undoOnNextExecution(bool undo)
{
    if (canUndo() && undo && !isUndoSet())
        m_request.setCommand(QStringLiteral("undo_") + m_request.command(),
                             m_request.params(), m_request.queryGroup());
    else if (isUndoSet())
        m_request.setCommand(m_request.command().remove("undo_"),
                             m_request.params(), m_request.queryGroup());
}

QString QueryExecutor::connectionName() const
{
    return m_connectionName;
}
void QueryExecutor::setConnectionName(const QString& connectionName)
{
    m_connectionName = connectionName;
}

QVariantMap QueryExecutor::recordToMap(const QSqlRecord& record) noexcept
{
    QVariantMap map;
    for (int i = 0; i < record.count(); i++)
        map.insert(record.fieldName(i), record.value(i));

    return map;
}

QSqlRecord QueryExecutor::mapToRecord(const QVariantMap& map) noexcept
{
    QSqlRecord record;

    QMapIterator<QString, QVariant> mapIter(map);
    while (mapIter.hasNext()) {
        auto pair = mapIter.next();
        record.setValue(pair.key(), pair.value());
    }

    return record;
}

void QueryExecutor::enforceArguments(QStringList argumentsToEnforce,
                                     const QVariantMap& params)
{
    if (argumentsToEnforce.isEmpty())
        return;

    QMapIterator<QString, QVariant> mapIter(params);
    while (mapIter.hasNext()) {
        auto pair = mapIter.next();
        if (argumentsToEnforce.contains(pair.key()))
            argumentsToEnforce.removeAll(pair.key());
    }

    if (!argumentsToEnforce.isEmpty())
        throw MissingArgumentException(
            QStringLiteral("The following mandatory parameters are not set: %1")
                .arg(argumentsToEnforce.join(", ")));
}

QList<QSqlRecord> QueryExecutor::callProcedure(
    const QString& procedure,
    std::initializer_list<ProcedureArgument> arguments)
{
    if (procedure.trimmed().isEmpty())
        return QList<QSqlRecord>();

    QSqlDatabase connection = QSqlDatabase::database(m_connectionName);
    QSqlQuery q(connection);
    QList<QSqlRecord> records;
    QStringList sqlArguments;
    QStringList outArguments;
    QStringList selectStatementSuffixes;

    auto areAllArgumentsNull = [](const QSqlRecord& record,
                                  const QStringList& arguments) {
        int nullArgumentCount = 0;
        for (const QString& argument : arguments) {
            if (record.value(argument).isNull())
                nullArgumentCount++;
        }

        return nullArgumentCount == arguments.count();
    };

    for (const auto& argument : arguments) {
        switch (argument.type) {
            case ProcedureArgument::Type::In:
                if (argument.value.isNull())
                    sqlArguments.append(QStringLiteral("NULL"));
                else if (argument.value.type() == QVariant::Bool)
                    sqlArguments.append(argument.value.toBool() ? "TRUE"
                                                                : "FALSE");
                else if (argument.value.type() == QVariant::String)
                    sqlArguments.append(QStringLiteral("'%1'").arg(
                        argument.value.toString().replace("'", "\\'")));
                else if (argument.value.type() == QVariant::ByteArray)
                    sqlArguments.append(QStringLiteral("'%1'").arg(
                        QString(argument.value.toByteArray().toHex())));
                else if (argument.value.type() == QVariant::DateTime)
                    sqlArguments.append(QStringLiteral("'%1'").arg(
                        argument.value.toDateTime()
                            .toString("yyyy-MM-dd hh:mm:ss")
                            .replace("'", "\\'")));
                else if (argument.value.type() == QVariant::Date)
                    sqlArguments.append(
                        QStringLiteral("'%1'").arg(argument.value.toDateTime()
                                                       .toString("yyyy-MM-dd")
                                                       .replace("'", "\\'")));
                else if (argument.value.type() == QVariant::Time)
                    sqlArguments.append(
                        QStringLiteral("'%1'").arg(argument.value.toDateTime()
                                                       .toString("hh:mm:ss")
                                                       .replace("'", "\\'")));
                else if (argument.value.type() == QVariant::Map)
                    sqlArguments.append(QStringLiteral("'%1'").arg(
                        QString(QJsonDocument(QJsonObject::fromVariantMap(
                                                  argument.value.toMap()))
                                    .toJson())));
                else if (argument.value.type() == QVariant::List)
                    sqlArguments.append(QStringLiteral("'%1'").arg(
                        QString(QJsonDocument(QJsonArray::fromVariantList(
                                                  argument.value.toList()))
                                    .toJson())));
                else
                    sqlArguments.append(argument.value.toString());
                break;
            case ProcedureArgument::Type::Out:
                sqlArguments.append(QStringLiteral("@") + argument.name);
                outArguments.append(argument.name);
                selectStatementSuffixes.append(
                    QStringLiteral("@%1 AS %1").arg(argument.name));
                break;
            case ProcedureArgument::Type::InOut: {
                QString inArgument;
                if (argument.value.type() == QVariant::String ||
                    argument.value.type() == QVariant::ByteArray)
                    inArgument = QStringLiteral("'%1'").arg(
                        argument.value.toString().replace("'", "\\'"));
                else
                    inArgument =
                        QStringLiteral("%1").arg(argument.value.toString());

                if (!q.exec(QStringLiteral("SET @%1 = %2")
                                .arg(argument.name, inArgument)))
                    throw ProcedureCallFailedException(
                        QStringLiteral("Failed to SET variable @%1")
                            .arg(argument.name),
                        q.lastError());

                sqlArguments.append(QStringLiteral("@") + argument.name);
                outArguments.append(argument.name);
                selectStatementSuffixes.append(
                    QStringLiteral("@%1 AS %1").arg(argument.name));
            } break;
        }
    }

    const auto& storedProcedure = QStringLiteral("SELECT * FROM %1(%2)")
                                      .arg(procedure, sqlArguments.join(", "));
    qCInfo(lcqueryexecutor) << "Procedure syntax:" << storedProcedure;
    if (!q.exec(storedProcedure)) {
        if (q.lastError().nativeErrorCode().toInt() >=
            static_cast<int>(
                DatabaseError::MySqlErrorCode::UserDefinedException))
            throw ProcedureCallFailedException(q.lastError().text(),
                                               q.lastError());
        else
            throw ProcedureCallFailedException(
                QStringLiteral("Procedure '%1' failed.").arg(procedure),
                q.lastError());
    }

    if (!selectStatementSuffixes.isEmpty()) {
        if (!q.exec(QStringLiteral("SELECT %1")
                        .arg(selectStatementSuffixes.join(", "))))
            throw ProcedureCallFailedException(
                QStringLiteral(
                    "Failed to select out arguments for procedure '%1'.")
                    .arg(procedure),
                q.lastError());

        while (q.next()) {
            if (areAllArgumentsNull(q.record(), outArguments))
                return QList<QSqlRecord>();

            records.append(q.record());
        }
    } else if (outArguments.isEmpty()) {
        while (q.next()) {
            records.append(q.record());
        }
    }

    return records;
}

int QueryExecutor::addNote(const QString& note, const QString& tableName,
                           ExceptionPolicy policy)
{
    try {
        if (note.trimmed().isEmpty())
            throw InvalidArgumentException("Cannot add empty string as note.");
        if (tableName.trimmed().isEmpty())
            throw InvalidArgumentException(
                "Cannot add note with no table name.");

        const auto& records(callProcedure(
            "AddNote",
            {ProcedureArgument{ProcedureArgument::Type::In, "note", note},
             ProcedureArgument{ProcedureArgument::Type::In, "table_name",
                               "debtor"},
             ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                               UserProfile::instance().userId()}}));

        return records.first().value("id").toInt();
    } catch (const DatabaseException&) {
        if (policy == ExceptionPolicy::AllowExceptions)
            throw;
    }

    return 0;
}

void QueryExecutor::updateNote(int noteId, const QString& note,
                               const QString& tableName, ExceptionPolicy policy)
{
    try {
        if (noteId <= 0)
            throw InvalidArgumentException(
                QStringLiteral("Cannot update note with invalid ID %1")
                    .arg(noteId));
        if (note.trimmed().isEmpty())
            throw InvalidArgumentException(
                "Cannot update note with empty string.");

        callProcedure(
            "UpdateNote",
            {ProcedureArgument{ProcedureArgument::Type::In, "note_id", noteId},
             ProcedureArgument{ProcedureArgument::Type::In, "note", note},
             ProcedureArgument{ProcedureArgument::Type::In, "table_name",
                               tableName},
             ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                               UserProfile::instance().userId()}});
    } catch (const DatabaseException&) {
        if (policy == ExceptionPolicy::AllowExceptions)
            throw;
    }
}

int QueryExecutor::addOrUpdateNote(int noteId, const QString& note,
                                   const QString& tableName,
                                   ExceptionPolicy policy)
{
    try {
        if (!note.trimmed().isEmpty()) {
            if (noteId <= 0)
                noteId = addNote(note, tableName);
            else
                updateNote(noteId, note, tableName);
        }

        return noteId;
    } catch (const DatabaseException&) {
        if (policy == ExceptionPolicy::AllowExceptions)
            throw;
    }

    return 0;
}

void QueryExecutor::beginTransaction(QSqlQuery& q)
{
    if (!q.exec("START TRANSACTION"))
        throw BeginTransactionFailedException(q.lastError());
}

void QueryExecutor::commitTransaction(QSqlQuery& q)
{
    if (!q.exec("COMMIT"))
        throw CommitTransactionFailedException(q.lastError());
}

void QueryExecutor::rollbackTransaction(QSqlQuery& q)
{
    if (!q.exec("ROLLBACK"))
        qCritical("Failed to rollback failed transaction! %s",
                  q.lastError().text().toStdString().c_str());
}

QByteArray QueryExecutor::imageUrlToByteArray(const QUrl& imageUrl,
                                              qint64 maxSize)
{
    if (imageUrl.isEmpty())
        return QByteArray();

    QImage image(imageUrl.toLocalFile());
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    if (ba.size() > maxSize)
        throw ImageTooLargeException(ba.size(), maxSize);
    return ba;
}

QUrl QueryExecutor::byteArrayToImageUrl(const QByteArray& imageData)
{
    if (imageData.isNull())
        return QString();

    const QString& imageSource = generateFileName(imageData);
    QFile file(imageSource);
    file.open(QIODevice::WriteOnly);
    file.write(QByteArray::fromHex(imageData));

    return QUrl::fromLocalFile(imageSource).toString();
}

QString QueryExecutor::generateFileName(const QByteArray& imageData)
{
    QByteArray ba;
    if (imageData.size() < HASH_INPUT_LENGTH) {
        for (int i = 0; i < imageData.size(); ++i)
            ba.append(imageData.at(i));
    } else {
        for (int i = 0; i < HASH_INPUT_LENGTH; ++i)
            ba.append(imageData.at(i));
    }

    return QStringLiteral("%1/%2.png")
        .arg(
            QStandardPaths::writableLocation(QStandardPaths::TempLocation),
            QString(
                QCryptographicHash::hash(ba, QCryptographicHash::Md5).toHex()));
}
