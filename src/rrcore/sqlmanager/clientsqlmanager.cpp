#include "clientsqlmanager.h"

#include "database/databaseexception.h"

#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlError>

ClientSqlManager::ClientSqlManager(const QString &connectionName) :
    AbstractSqlManager(connectionName)
{

}

QueryResult ClientSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "view_clients")
            viewClients(request, result);
        else
            throw DatabaseException(DatabaseException::RRErrorCode::CommandNotFound, QString("Command not found: %1").arg(request.command()));

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

void ClientSqlManager::viewClients(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    QSqlQuery q(connection);

    try {
        if (params.value("filter_column").toString() == "preferred_name") {
            q.prepare(QString("SELECT client.id AS client_id, client.preferred_name AS preferred_name, "
                              "client.phone_number AS phone_number FROM client "
                              "WHERE client.archived = :archived AND client.preferred_name LIKE '%%1%'")
                      .arg(params.value("filter_text").toString()));
            q.bindValue(":archived", params.value("archived", false), QSql::Out);

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::ViewClientsFailure,
                                        q.lastError().text(),
                                        QStringLiteral("Failed to fetch clients with 'preferred name' filter."));
        } else if (params.value("filter_column").toString() == "phone_number") {
            q.prepare(QString("SELECT client.id AS client_id, client.preferred_name AS preferred_name, "
                              "client.phone_number AS phone_number FROM client "
                              "WHERE client.archived = :archived AND client.phone_number LIKE '%%1%'")
                      .arg(params.value("filter_text").toString()));
            q.bindValue(":archived", params.value("archived", false), QSql::Out);

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::ViewClientsFailure,
                                        q.lastError().text(),
                                        QStringLiteral("Failed to fetch clients with 'phone number' filter."));
        } else {
            const QString &storedProcedure = QString("CALL ViewClients(%1, @id, @preferred_name, @phone_number)")
                    .arg(params.value("archived", false).toString());
            if (!q.exec(storedProcedure))
                throw DatabaseException(DatabaseException::RRErrorCode::ViewClientsFailure,
                                        q.lastError().text(),
                                        QStringLiteral("Procedure ViewClients() failed."));
            if (!q.exec("SELECT @id AS client_id,"
                        "@preferred_name AS preferred_name,"
                        "@phone_number AS phone_number"))
                throw DatabaseException(DatabaseException::RRErrorCode::ViewClientsFailure,
                                        q.lastError().text(),
                                        QStringLiteral("Failed to fetch clients."));
        }

        QVariantList clients;
        while (q.next()) {
            clients.append(recordToMap(q.record()));
        }

        result.setOutcome(QVariantMap { { "clients", clients }, { "record_count", clients.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}
