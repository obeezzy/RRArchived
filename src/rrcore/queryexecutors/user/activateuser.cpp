#include "activateuser.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace UserQuery;

ActivateUser::ActivateUser(const QString &userName,
                           bool active,
                           QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user_name", userName },
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
                              "user_name",
                              params.value("user_name")
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
