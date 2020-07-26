#include "activateuser.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "database/databaseexception.h"
#include "utility/user/user.h"

using namespace Query::User;

ActivateUser::ActivateUser(bool active, const Utility::User::User& user,
                           QObject* receiver)
    : UserExecutor(COMMAND, {{"user", user.user}, {"active", active}}, receiver)
{}

QueryResult ActivateUser::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    const QVariantMap& params{request().params()};

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        callProcedure("ActivateUser",
                      {ProcedureArgument{ProcedureArgument::Type::In, "user",
                                         params.value("user")},
                       ProcedureArgument{ProcedureArgument::Type::In, "active",
                                         params.value("active")}});

        QueryExecutor::commitTransaction(q);
    } catch (const DatabaseException&) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
    return result;
}
