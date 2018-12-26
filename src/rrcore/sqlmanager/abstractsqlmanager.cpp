#include "abstractsqlmanager.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

AbstractSqlManager::AbstractSqlManager(const QString &connectionName) :
    m_connectionName(connectionName)
{

}

AbstractSqlManager::~AbstractSqlManager()
{

}

QVariantMap AbstractSqlManager::recordToMap(const QSqlRecord &record)
{
    QVariantMap map;
    for (int i = 0; i < record.count(); i++)
        map.insert(record.fieldName(i), record.value(i));

    return map;
}

QSqlRecord AbstractSqlManager::mapToRecord(const QVariantMap &map)
{
    QSqlRecord record;

    QMapIterator<QString, QVariant> mapIter(map);
    while (mapIter.hasNext()) {
        auto pair = mapIter.next();
        record.setValue(pair.key(), pair.value());
    }

    return record;
}

void AbstractSqlManager::enforceArguments(QStringList argumentsToEnforce, const QVariantMap &params)
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
        throw DatabaseException(DatabaseException::RRErrorCode::MissingArguments,
                                QString(),
                                QString("The following mandatory parameters are not set: %1").arg(argumentsToEnforce.join(", ")));
}

QList<QSqlRecord> AbstractSqlManager::callProcedure(const QString &procedure, std::initializer_list<ProcedureArgument> arguments)
{
    if (procedure.trimmed().isEmpty())
        return QList<QSqlRecord>();

    QSqlDatabase connection = QSqlDatabase::database(m_connectionName);
    QSqlQuery q(connection);
    QList<QSqlRecord> records;
    QStringList sqlArguments;
    QStringList outArguments;
    QStringList selectStatementSuffixes;

    auto areAllArgumentsNull = [](const QSqlRecord &record, const QStringList &arguments) {
        int nullArgumentCount = 0;
        for (const QString &argument : arguments) {
            if (record.value(argument).isNull())
                nullArgumentCount++;
        }

        return nullArgumentCount == arguments.count();
    };

    for (const auto &argument : arguments) {
        switch (argument.type) {
        case ProcedureArgument::Type::In:
            if (argument.value.isNull())
                sqlArguments.append(QStringLiteral("NULL"));
            else if (argument.value.type() == QVariant::Bool)
                sqlArguments.append(argument.value.toBool() ? "1" : "0");
            else if (argument.value.type() == QVariant::String
                     || argument.value.type() == QVariant::ByteArray
                     || argument.value.type() == QVariant::DateTime
                     || argument.value.type() == QVariant::Date
                     || argument.value.type() == QVariant::Time)
                sqlArguments.append(QStringLiteral("'%1'").arg(argument.value.toString()));
            else
                sqlArguments.append(argument.value.toString());
            break;
        case ProcedureArgument::Type::Out:
            sqlArguments.append(QString("@") + argument.name);
            outArguments.append(argument.name);
            selectStatementSuffixes.append(QString("@%1 AS %1").arg(argument.name));
            break;
        case ProcedureArgument::Type::InOut:
        {
            QString inArgument;
            if (argument.value.type() == QVariant::String || argument.value.type() == QVariant::ByteArray)
                inArgument = QStringLiteral("'%1'").arg(argument.value.toString());
            else
                inArgument = QStringLiteral("%1").arg(argument.value.toString());

            if (!q.exec(QStringLiteral("SET @%1 = %2").arg(argument.name, inArgument)))
                throw DatabaseException(DatabaseException::RRErrorCode::ProcedureFailed,
                                        QStringLiteral("Failed to SET variable @%1").arg(argument.name));

            sqlArguments.append(QString("@") + argument.name);
            outArguments.append(argument.name);
            selectStatementSuffixes.append(QString("@%1 AS %1").arg(argument.name));
        }
            break;
        }
    }

    const QString &storedProcedure = QString("CALL %1(%2)").arg(procedure, sqlArguments.join(", "));
    qInfo() << "Procedure syntax: " << storedProcedure;
    if (!q.exec(storedProcedure))
        throw DatabaseException(DatabaseException::RRErrorCode::ProcedureFailed,
                                q.lastError().text(),
                                QStringLiteral("Procedure '%1' failed.").arg(procedure));

    if (!selectStatementSuffixes.isEmpty()) {
        if (!q.exec(QStringLiteral("SELECT %1").arg(selectStatementSuffixes.join(", "))))
            throw DatabaseException(DatabaseException::RRErrorCode::ProcedureFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to select out arguments for procedure '%1'.").arg(procedure));

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
