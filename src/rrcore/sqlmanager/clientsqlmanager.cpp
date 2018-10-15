#include "clientsqlmanager.h"

#include "database/databaseexception.h"

#include <QSqlQuery>
#include <QSqlError>

ClientSqlManager::ClientSqlManager(QSqlDatabase connection) :
    AbstractSqlManager(connection)
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
    const QVariantMap &params = request.params();
    QSqlQuery q(connection());

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
//            q.prepare("SELECT client.id AS client_id, client.preferred_name AS preferred_name, "
//                      "client.phone_number AS phone_number FROM client "
//                      "WHERE client.archived = :archived");
//            q.bindValue(":archived", params.value("archived", false), QSql::Out);

//            if (!q.exec())
//                throw DatabaseException(DatabaseException::RRErrorCode::ViewClientsFailure,
//                                        q.lastError().text(),
//                                        QStringLiteral("Failed to fetch clients."));
            q.prepare("CALL ViewClients(:archived)");
            q.bindValue(":archived", params.value("archived", false), QSql::In);

            if (!q.exec())
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
