#include "changepassword.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>

using namespace UserQuery;

ChangePassword::ChangePassword(const QString &oldPassword, const QString &newPassword, QObject *receiver) :
    UserExecutor(COMMAND, {
                { "user_name", UserProfile::instance().userName() },
                { "rack_id", UserProfile::instance().rackId() },
                { "old_password", oldPassword },
                { "new_password", newPassword }
            }, receiver)
{

}

QueryResult ChangePassword::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        callProcedure("ChangePassword", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_name",
                              params.value("user_name").toString()
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "new_password",
                              params.value("new_password")
                          }
                      });

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &e) {
        DatabaseUtils::rollbackTransaction(q);

        switch (e.code()) {
        case DatabaseError::asInteger(DatabaseError::MySqlErrorCode::UserDefinedException):
            throw DatabaseException(DatabaseError::QueryErrorCode::OldPasswordWrong, e.message(), e.userMessage());
        default:
            throw;
        }
    }

    return result;
}
