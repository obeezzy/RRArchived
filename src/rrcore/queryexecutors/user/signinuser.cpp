#include "signinuser.h"
#include <QSqlDatabase>
#include "config/config.h"
#include <QSqlError>
#include <QLoggingCategory>
#include "database/databaseerror.h"
#include "database/databaseexception.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "user/userprofile.h"

Q_LOGGING_CATEGORY(signInUser, "rrcore.queryexecutors.user.signinuser");

using namespace UserQuery;

SignInUser::SignInUser(const QString &userName,
                       const QString &password,
                       QObject *receiver) :
    UserExecutor(COMMAND, {
                    { "user_name", userName },
                    { "password", password },
                    { "rack_id", UserProfile::instance().rackId() },
                    { "user_id", UserProfile::instance().userId() }
                  }, receiver)
{
}

QueryResult SignInUser::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection;
    const QString &userName = request().params().value("user_name").toString();
    const QString &password = request().params().value("password").toString();

    QueryExecutor::enforceArguments({ "user_name", "password" }, request().params());

    if (!QSqlDatabase::contains())
        connection = QSqlDatabase::addDatabase("QMYSQL", connectionName());
    else
        connection = QSqlDatabase::database(connectionName());

    connection.setDatabaseName(Config::instance().databaseName());
    connection.setHostName(Config::instance().hostName());
    connection.setPort(Config::instance().port());
    connection.setUserName(UserExecutor::resolvedUserName(userName));
    connection.setPassword(password);
    connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    if (!connection.open() || !storeProfile(result, userName, password)) {
        if (connection.lastError().nativeErrorCode().toInt() == static_cast<int>(DatabaseError::MySqlErrorCode::UserAccountIsLockedError))
            throw DatabaseException(DatabaseError::QueryErrorCode::UserAccountIsLocked, connection.lastError().text(),
                                    QString("Failed to sign in as '%1'.").arg(userName));
        else
            throw DatabaseException(DatabaseError::QueryErrorCode::SignInFailure, connection.lastError().text(),
                                    QString("Failed to sign in as '%1'.").arg(userName));
    }

    return result;
}

bool SignInUser::storeProfile(QueryResult &result, const QString &userName, const QString &password)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());

    if (!connection.isOpen())
        throw DatabaseException(DatabaseError::QueryErrorCode::UnknownError,
                                QStringLiteral("Database connection is closed."));

    try {
        const QList<QSqlRecord> &records(callProcedure("GetUserDetails", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_name",
                                                               userName
                                                           }
                                                       }));

        QVariantMap userDetails;
        if (!records.isEmpty())
            userDetails = recordToMap(records.first());
        else
            throw DatabaseException(DatabaseError::QueryErrorCode::UnknownError,
                                    QStringLiteral("User does not exist."));

        result.setOutcome(QVariantMap {
                              { "user_name", userName },
                              { "password", password },
                              { "user_id", userDetails.value("user_id").toInt() },
                              { "user_privileges", userDetails.value("user_privileges") },
                              { "record_count", 1 }
                          });
    } catch (DatabaseException &e) {
        qCWarning(signInUser) << e;
        throw;
    }

    return true;
}
