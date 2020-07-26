#include "removeuser.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "database/databaseexception.h"
#include "utility/user/user.h"

using namespace Query::User;

RemoveUser::RemoveUser(const Utility::User::User& user, QObject* receiver)
    : UserExecutor(COMMAND, {{"user", user.user}}, receiver)
{}

QueryResult RemoveUser::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    const QVariantMap& params = request().params();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        callProcedure("RemoveUser",
                      {ProcedureArgument{ProcedureArgument::Type::In, "user",
                                         params.value("user")}});

        QueryExecutor::commitTransaction(q);
        return result;
    } catch (const DatabaseException&) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
}
