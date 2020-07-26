#include "fetchuser.h"
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"

using namespace Query::User;

FetchUser::FetchUser(int userId,
                     QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user_id", userId },
                    { "archived", false }
                 }, receiver)
{

}

QueryResult FetchUser::execute()
{
    QueryResult result { request() };
    const QVariantMap &params = request().params();

    try {
        const auto &records(callProcedure("FetchUser", {
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

        QVariantMap user;
        if (records.isEmpty())
            throw UnexpectedResultException(QStringLiteral("Expected user ID, received nothing."));

        user = recordToMap(records.first());
        result.setOutcome(QVariantMap {
                              { "user", user },
                              { "record_count", records.count() }
                          });

        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
