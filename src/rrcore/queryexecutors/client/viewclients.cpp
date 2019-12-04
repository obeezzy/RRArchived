#include "viewclients.h"
#include "database/databaseexception.h"

using namespace ClientQuery;

ViewClients::ViewClients(QObject *receiver) :
    ClientExecutor(COMMAND, { }, receiver)
{

}

ViewClients::ViewClients(const QString &filterText, const QString &filterColumn, QObject *receiver) :
    ClientExecutor(COMMAND, {
                        { "filter_text", filterText },
                        { "filter_column", filterColumn },
                        { "archived", false }
                   }, receiver)
{

}

QueryResult ViewClients::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> records(callProcedure("ViewClients", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "filter_column",
                                                              params.value("filter_column")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "filter_text",
                                                              params.value("filter_text")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "archived",
                                                              params.value("archived")
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
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
