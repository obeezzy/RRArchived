#include "activateuser.h"
#include "database/databaseexception.h"
#include "utility/userutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace UserQuery;

ActivateUser::ActivateUser(bool active,
                           const Utility::User &user,
                           QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user", user.user },
                    { "active", active }
                 }, receiver)
{

}

QueryResult ActivateUser::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params{ request().params() };

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        callProcedure("ActivateUser", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user",
                              params.value("user")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "active",
                              params.value("active")
                          }
                      });

        QueryExecutor::commitTransaction(q);
    } catch (const DatabaseException &) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
    return result;
}
