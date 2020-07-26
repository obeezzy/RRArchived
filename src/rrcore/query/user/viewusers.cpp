#include "viewusers.h"
#include "database/databaseexception.h"
#include "utility/common/recordgroup.h"

using namespace Query::User;

ViewUsers::ViewUsers(const Utility::RecordGroup::Flags &flags,
                     QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "archived", flags.testFlag(Utility::RecordGroup::Archived) }
                 }, receiver)
{

}

QueryResult ViewUsers::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const auto &records(callProcedure("ViewUsers", {
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "archived",
                                                  params.value("archived")
                                              }
                                          }));

        QVariantList users;
        for (const auto &record : records)
            users.append(recordToMap(record));

        result.setOutcome(QVariantMap {
                              { "users", users },
                              { "record_count", users.count() }
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
