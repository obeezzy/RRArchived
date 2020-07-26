#include "changepassword.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include "user/userprofile.h"

using namespace Query::User;

ChangePassword::ChangePassword(const QString& oldPassword,
                               const QString& newPassword, QObject* receiver)
    : UserExecutor(COMMAND,
                   {{"user", UserProfile::instance().userName()},
                    {"rack_id", UserProfile::instance().rackId()},
                    {"old_password", oldPassword},
                    {"new_password", newPassword}},
                   receiver)
{}

QueryResult ChangePassword::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap& params{request().params()};

    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        callProcedure(
            "ChangePassword",
            {ProcedureArgument{ProcedureArgument::Type::In, "user",
                               params.value("user").toString()},
             ProcedureArgument{ProcedureArgument::Type::In, "new_password",
                               params.value("new_password")}});

        QueryExecutor::commitTransaction(q);
    } catch (const DatabaseException& e) {
        QueryExecutor::rollbackTransaction(q);

        switch (e.code()) {
            case DatabaseError::asInteger(
                DatabaseError::MySqlErrorCode::UserDefinedException):
                throw IncorrectPasswordException(
                    QStringLiteral("The old password provided is wrong."),
                    e.message());
            default:
                throw;
        }
    }

    return result;
}
