#include "signinuser.h"
#include "config/config.h"
#include "database/databaseerror.h"
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "user/userprofile.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcsigninuser, "rrcore.queryexecutors.user.signinuser");

using namespace Query::User;

SignInUser::SignInUser(const Utility::User::User &user,
                       QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user", user.user },
                    { "password", user.password },
                    { "rack_id", UserProfile::instance().rackId() },
                    { "user_id", UserProfile::instance().userId() }
                 }, receiver)
{
}

QueryResult SignInUser::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QueryExecutor::enforceArguments({ "user",
                                      "password"
                                    }, request().params());

    attemptSignIn(result);

    return result;
}

bool SignInUser::storeProfile(QueryResult &result,
                              const QString &userName,
                              const QString &password)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());

    if (!connection.isOpen())
        throw ConnectionFailedException(QStringLiteral("Database connection is closed."),
                                        connection.lastError());

    try {
        const auto &records(callProcedure("FetchUserByName", {
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "user",
                                                  userName
                                              }
                                          }));

        if (records.isEmpty())
            throw NoExistingRecordException(QStringLiteral("User '%1' does not exist in RR user table.")
                                            .arg(userName));

        const QVariantMap &user{ recordToMap(records.first()) };
        result.setOutcome(QVariantMap {
                              { "user", user },
                              { "password", password },
                              { "user_id", user.value("user_id").toInt() },
                              { "user_privileges", user.value("user_privileges") },
                              { "record_count", 1 }
                          });
    } catch (const DatabaseException &e) {
        throw;
    }

    return true;
}

void SignInUser::attemptSignIn(QueryResult &result)
{
    QSqlDatabase connection;
    const QString &userName{ request().params().value("user").toString() };
    const QString &password{ request().params().value("password").toString() };

    if (!QSqlDatabase::contains(connectionName()))
       connection = QSqlDatabase::addDatabase("QPSQL", connectionName());
    else
        connection = QSqlDatabase::database(connectionName());

    connection.setDatabaseName(Config::instance().databaseName());
    connection.setHostName(Config::instance().hostName());
    connection.setPort(Config::instance().port());
    connection.setUserName(userName);
    connection.setPassword(password);
    connection.setConnectOptions();

    if (!connection.open() || !storeProfile(result, userName, password)) {
        if (connection.lastError().nativeErrorCode().toInt()
                == static_cast<int>(DatabaseError::MySqlErrorCode::UserAccountIsLockedError))
            throw UserAccountLockedException(QStringLiteral("User account for '%1' is locked.")
                                             .arg(userName),
                                             connection.lastError());
        else
            throw InvalidCredentialsException(QStringLiteral("Failed to sign in as '%1'.")
                                              .arg(userName),
                                              connection.lastError());
    }
}
