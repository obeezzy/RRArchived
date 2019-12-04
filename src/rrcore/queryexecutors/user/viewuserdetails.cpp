#include "viewuserdetails.h"
#include "database/databaseexception.h"

using namespace UserQuery;

ViewUserDetails::ViewUserDetails(int userId,
                                 QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user_id", userId },
                    { "archived", false }
                 }, receiver)
{

}

QueryResult ViewUserDetails::execute()
{
    QueryResult result { request() };
    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewUserDetails", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_id",
                                                               params.value("user_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "archived",
                                                               params.value("archived")
                                                           }
                                                       }));

        QVariantMap userDetails;
        if (!records.isEmpty()) {
            userDetails = recordToMap(records.first());
        }

        result.setOutcome(QVariantMap {
                              { "user", userDetails },
                              { "record_count", records.count() }
                          });

        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
