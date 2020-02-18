#include "removeuser.h"
#include "database/databaseexception.h"
#include "utility/userutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>

using namespace UserQuery;

RemoveUser::RemoveUser(const Utility::User &user,
                       QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user", user.user }
                 }, receiver)
{

}

QueryResult RemoveUser::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        callProcedure("RemoveUser", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user",
                              params.value("user")
                          }
                      });

        QueryExecutor::commitTransaction(q);
        return result;
    } catch (const DatabaseException &) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
}
