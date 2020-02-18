#include "viewclients.h"
#include "database/databaseexception.h"
#include "utility/commonutils.h"

using namespace ClientQuery;

ViewClients::ViewClients(QObject *receiver) :
    ClientExecutor(COMMAND, {
                    { "archived", false }
                   },
                   receiver)
{

}

ViewClients::ViewClients(const Utility::RecordGroup::Flags &flags,
                         QObject *receiver) :
    ClientExecutor(COMMAND, {
                    { "archived", flags.testFlag(Utility::RecordGroup::Archived) }
                   }, receiver)
{

}

QueryResult ViewClients::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const auto &records(callProcedure("ViewClients", {
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "archived",
                                                  params.value("archived")
                                              }
                                          }));


        QVariantList clients;
        for (const auto &record : records)
            clients.append(recordToMap(record));

        result.setOutcome(QVariantMap {
                              { "clients", clients },
                              { "record_count", clients.count() }
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
