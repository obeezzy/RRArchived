#include "updateuserprivileges.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace UserQuery;

UpdateUserPrivileges::UpdateUserPrivileges(int userId,
                                           QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user_id", userId }
                 }, receiver)
{

}

QueryResult UpdateUserPrivileges::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        callProcedure("UpdateUserPrivileges", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_privileges",
                              params.value("user_privileges")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              params.value("user_id")
                          }
                      });

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &e) {
        DatabaseUtils::rollbackTransaction(q);

        switch (e.code()) {
        case DatabaseError::asInteger(DatabaseError::MySqlErrorCode::DuplicateEntryError):
            throw DatabaseException(DatabaseError::QueryErrorCode::DuplicateEntryFailure, e.message(), e.userMessage());
        case DatabaseError::asInteger(DatabaseError::MySqlErrorCode::CreateUserError):
            throw DatabaseException(DatabaseError::QueryErrorCode::CreateUserFailed, e.message(), e.userMessage());
        default:
            throw;
        }
    }
    return result;
}
