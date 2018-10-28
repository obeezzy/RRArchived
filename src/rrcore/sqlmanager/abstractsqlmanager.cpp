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

QList<QSqlRecord> AbstractSqlManager::callProcedure(const QString &procedure, std::initializer_list<ProcedureArgument> arguments,
                                                    const QStringList &outArgumentsThatMustNotBeNull)
{
    if (procedure.trimmed().isEmpty())
        return QList<QSqlRecord>();

    QSqlDatabase connection = QSqlDatabase::database(m_connectionName);
    QSqlQuery q(connection);
    QList<QSqlRecord> records;
    QStringList sqlArguments;
    QStringList outArguments;

    for (const auto &argument : arguments) {
        switch (argument.type) {
        case ProcedureArgument::Type::In:
            if (argument.value.isNull())
                sqlArguments.append(QStringLiteral("NULL"));
            else if (argument.value.type() == QVariant::Bool)
                sqlArguments.append(argument.value.toBool() ? "1" : "0");
            else if (argument.value.type() == QVariant::String)
                sqlArguments.append(QStringLiteral("'%1'").arg(argument.value.toString()));
            else
                sqlArguments.append(argument.value.toString());
            break;
        case ProcedureArgument::Type::Out:
            sqlArguments.append(QString("@") + argument.name);
            outArguments.append(QString("@%1 AS %1").arg(argument.name));
            break;
        case ProcedureArgument::Type::InOut:
            break;
        }
    }

    const QString &storedProcedure = QString("CALL %1(%2)").arg(procedure, sqlArguments.join(", "));
    qInfo() << "Procedure syntax: " << storedProcedure;
    if (!q.exec(storedProcedure))
        throw DatabaseException(DatabaseException::RRErrorCode::ProcedureFailed,
                                q.lastError().text(),
                                QStringLiteral("Procedure '%1' failed.").arg(procedure));

    if (!outArguments.isEmpty()) {
        if (!q.exec(QStringLiteral("SELECT %1").arg(outArguments.join(", "))))
            throw DatabaseException(DatabaseException::RRErrorCode::ProcedureFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to select out arguments for procedure '%1'.").arg(procedure));

        while (q.next()) {
            for (const QString &outArgument : outArgumentsThatMustNotBeNull) {
                if (q.record().value(outArgument).isNull())
                    return QList<QSqlRecord>();
            }

            records.append(q.record());
        }
    }

    return records;
}
