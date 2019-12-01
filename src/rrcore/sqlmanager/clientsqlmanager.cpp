#include "clientsqlmanager.h"
#include "database/databaseexception.h"

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
            throw DatabaseException(DatabaseError::QueryErrorCode::CommandNotFound, QString("Command not found: %1").arg(request.command()));

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

    try {
        const QList<QSqlRecord> records(callProcedure("ViewClients", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "filter_column",
                                                              params.value("filter_column", QVariant::String)
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "filter_text",
                                                              params.value("filter_text", QVariant::String)
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "archived",
                                                              params.value("archived", false)
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "client_id",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "preferred_name",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "phone_number",
                                                              {}
                                                          }
                                                      }));


        QVariantList clients;
        for (const QSqlRecord &record : records) {
            clients.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap { { "clients", clients }, { "record_count", clients.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}
