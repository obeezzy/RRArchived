#include "viewusers.h"
#include "database/databaseexception.h"

using namespace UserQuery;

ViewUsers::ViewUsers(bool archived,
                     QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "archived", archived }
                 }, receiver)
{

}

QueryResult ViewUsers::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewUsers", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "archived",
                                                               params.value("archived")
                                                           }
                                                       }));

        QVariantList users;
        for (const QSqlRecord &record : records) {
            users.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap {
                              { "users", users },
                              { "record_count", users.count() }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
