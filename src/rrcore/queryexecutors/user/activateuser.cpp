#include "activateuser.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "utility/userutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace UserQuery;

ActivateUser::ActivateUser(bool active,
                           const User &user,
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
    const QVariantMap &params = request().params();
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

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

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
    return result;
}
