#include "usersqlmanager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

#include "database/queryrequest.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "config/config.h"
#include "singletons/userprofile.h"
#include "json/userprivilegecenter.h"

UserSqlManager::UserSqlManager(const QString &connectionName)
    : AbstractSqlManager(connectionName)
{

}

QueryResult UserSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "sign_in_user")
            signInUser(request, result);
        else if (request.command() == "sign_up_user")
            signUpUser(request);
        else if (request.command() == "sign_up_root_user")
            signUpRootUser(request);
        else if (request.command() == "remove_user")
            removeUser(request);
        else if (request.command() == "view_users")
            viewUsers(request, result);
        else if (request.command() == "view_user_privileges")
            viewUserPrivileges(request, result);
        else if (request.command() == "add_user")
            addUser(request);
        else if (request.command() == "activate_user")
            activateUser(request);
        else if (request.command() == "view_user_details")
            viewUserDetails(request, result);
        else if (request.command() == "update_user_privileges")
            updateUserPrivileges(request);
        else
            throw DatabaseException(DatabaseException::RRErrorCode::CommandNotFound, QString("Command not found: %1").arg(request.command()));

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

bool UserSqlManager::storeProfile(QueryResult &result, const QString &userName)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());

    if (!connection.isOpen()) {
        qWarning() << "Database connection is closed.";
        throw DatabaseException(DatabaseException::RRErrorCode::Unknown,
                                QStringLiteral("Database connection is closed."));
    }

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
            throw DatabaseException(DatabaseException::RRErrorCode::Unknown,
                                    QStringLiteral("User does not exist."));

        result.setOutcome(QVariantMap {
                              { "user_name", userName },
                              { "user_id", userDetails.value("user_id").toInt() },
                              { "user_privileges", userDetails.value("user_privileges") },
                              { "record_count", 1 }
                          });
    } catch (DatabaseException &e) {
        qWarning() << "Exception thrown while storing profile:" << e.code() << e.message() << e.userMessage();
        throw;
    }

    return true;
}

void UserSqlManager::signInUser(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection;
    const QString &userName = request.params().value("user_name").toString();
    const QString &password = request.params().value("password").toString();

    if (!QSqlDatabase::contains())
        connection = QSqlDatabase::addDatabase("QMYSQL", connectionName());
    else
        connection = QSqlDatabase::database(connectionName());

    connection.setDatabaseName(Config::instance().databaseName());
    connection.setHostName(Config::instance().hostName());
    connection.setPort(Config::instance().port());
    connection.setUserName(userName);
    connection.setPassword(password);
    connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    if (!connection.open() || !storeProfile(result, userName))
        throw DatabaseException(DatabaseException::RRErrorCode::SignInFailure, connection.lastError().text(),
                                QString("Failed to sign in as '%1'.").arg(userName));
}

void UserSqlManager::signUpUser(const QueryRequest &request)
{
    QSqlDatabase connection;
    const QString &userName = request.params().value("user_name").toString();
    const QString &password = request.params().value("password").toString();

    if (connection.isOpen())
        connection.close();

    if (!QSqlDatabase::contains())
        connection = QSqlDatabase::addDatabase("QMYSQL", connectionName());
    else
        connection = QSqlDatabase::database(connectionName());

    connection.setDatabaseName(Config::instance().databaseName());
    connection.setHostName(Config::instance().hostName());
    connection.setPort(Config::instance().port());
    connection.setUserName(Config::instance().userName());
    connection.setPassword(Config::instance().password());
    connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    if (!connection.open())
        throw DatabaseException(DatabaseException::RRErrorCode::SignInFailure, connection.lastError().text(),
                                QString("Failed to open connection."));

    QSqlQuery q(connection);

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        q.prepare("FLUSH PRIVILEGES");
        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                    QString("Failed to flush privileges for '%1'.").arg(userName));

        q.prepare("CREATE USER :username @'localhost' IDENTIFIED BY :password");
        q.bindValue(":username", userName);
        q.bindValue(":password", password);
        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                    QString("Failed to create user '%1'.").arg(userName));

        grantPrivilege("SELECT", userName, q);
        grantPrivilege("INSERT", userName, q);
        grantPrivilege("UPDATE", userName, q);
        grantPrivilege("DELETE", userName, q);

        q.prepare("FLUSH PRIVILEGES");
        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                    QString("Failed to flush privileges for '%1'.").arg(userName));

        createRRUser(userName, q);

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit transation.");

    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void UserSqlManager::signUpRootUser(const QueryRequest &request)
{
    QSqlDatabase connection;
    const QString &userName = request.params().value("user_name").toString();
    const QString &password = request.params().value("password").toString();

    connection.setDatabaseName("mysql");

    if (!QSqlDatabase::contains())
        connection = QSqlDatabase::addDatabase("QMYSQL", connectionName());
    else
        connection = QSqlDatabase::database(connectionName());

    connection.setDatabaseName(Config::instance().databaseName());
    connection.setHostName(Config::instance().hostName());
    connection.setPort(Config::instance().port());
    connection.setUserName(userName);
    connection.setPassword(password);
    connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

    QSqlQuery q(connection);

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        q.prepare(QString("CREATE USER '%1'@'localhost' IDENTIFIED BY '%2'").arg(userName, password));
        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                    QString("Failed to create root user '%1'.").arg(userName));

        grantPrivilege("ALL PRIVILEGES", userName, q);

        q.prepare("FLUSH PRIVILEGES");
        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                    QString("Failed to flush privileges for '%1'.").arg(userName));

        createRRUser(userName, q);

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit transation.");

    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void UserSqlManager::removeUser(const QueryRequest &request)
{
    QSqlDatabase connection;
    const QString &userName = request.params().value("user_name").toString();

    if (!QSqlDatabase::contains())
        connection = QSqlDatabase::addDatabase("QMYSQL", connectionName());
    else
        connection = QSqlDatabase::database(connectionName());

    if (!connection.isOpen())
        throw DatabaseException(DatabaseException::RRErrorCode::RemoveUserFailure,
                                connection.lastError().text(), QString("Connection is closed."));

    if (UserProfile::instance().userId() > 1)
        throw DatabaseException(DatabaseException::RRErrorCode::InsufficientUserPrivileges,
                                "You must be a super user to perform this action.");

    QSqlQuery q(connection);
    q.prepare("DROP USER :user_name@'localhost'");
    q.bindValue(":user_name", userName);

    if (!q.exec())
        throw DatabaseException(DatabaseException::RRErrorCode::RemoveUserFailure,
                                q.lastError().text(),
                                QString("Failed to drop user '%1'.").arg(userName));
}

void UserSqlManager::viewUsers(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    QSqlQuery q(connection);

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewUsers", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "archived",
                                                               params.value("archived", false)
                                                           }
                                                       }));

        QVariantList users;
        for (const QSqlRecord &record : records) {
            users.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap {
                              { "users", users },
                              { "record_count", users.count() }
                          });
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void UserSqlManager::viewUserPrivileges(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewUserPrivileges", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_id",
                                                               params.value("user_id", QVariant::Int)
                                                           }
                                                       }));

        QVariantMap userPrivileges;
        if (!records.isEmpty()) {
            userPrivileges = QJsonDocument::fromJson(recordToMap(records.first())
                                                     .value("user_privileges").toString().toUtf8()).object().toVariantMap();
        } else {
            UserPrivilegeCenter userPrivilegeCenter;
            userPrivileges = userPrivilegeCenter.getPrivileges().toMap();
        }

        result.setOutcome(QVariantMap {
                              { "user_privileges", userPrivileges },
                              { "record_count", userPrivileges.count() }
                          });

    } catch (DatabaseException &) {
        throw;
    }
}

void UserSqlManager::addUser(const QueryRequest &request)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    QSqlQuery q(connection);

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        const QList<QSqlRecord> &records(callProcedure("AddRRUser", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "first_name",
                                                               params.value("first_name")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "last_name",
                                                               params.value("last_name")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_name",
                                                               params.value("user_name")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "photo",
                                                               DatabaseUtils::imageToByteArray(params.value("image_url").toString())
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "phone_number",
                                                               params.value("phone_number")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "email_address",
                                                               params.value("email_address")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "note_id",
                                                               params.value("note_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_id",
                                                               UserProfile::instance().userId()
                                                           }
                                                       }));

        if (!records.isEmpty()) {
            callProcedure("AddUserPrivileges", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_privileges",
                                  params.value("user_privileges")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  records.first().value("user_id")
                              }
                          });

        } else {
            throw DatabaseException(DatabaseException::RRErrorCode::AddUserFailed,
                                    "No user ID returned.");
        }


        callProcedure("AddSqlUser", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_name",
                              params.value("user_name")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "password",
                              params.value("password")
                          }
                      });

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit transation.");
    } catch (DatabaseException &e) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        if (e.code() == static_cast<int>(DatabaseException::MySqlErrorCode::DuplicateEntryError))
            throw DatabaseException(DatabaseException::RRErrorCode::DuplicateEntryFailure, e.message(), e.userMessage());
        else if (e.code() == static_cast<int>(DatabaseException::MySqlErrorCode::CreateUserError))
            throw DatabaseException(DatabaseException::RRErrorCode::CreateUserFailed, e.message(), e.userMessage());
        else
            throw;
    }
}

void UserSqlManager::activateUser(const QueryRequest &request)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    QSqlQuery q(connection);

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        callProcedure("ActivateUser", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              params.value("user_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "active",
                              params.value("active")
                          }
                      });

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit transation.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void UserSqlManager::updateUserPrivileges(const QueryRequest &request)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    QSqlQuery q(connection);

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

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

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit transation.");
    } catch (DatabaseException &e) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        if (e.code() == static_cast<int>(DatabaseException::MySqlErrorCode::DuplicateEntryError))
            throw DatabaseException(DatabaseException::RRErrorCode::DuplicateEntryFailure, e.message(), e.userMessage());
        else if (e.code() == static_cast<int>(DatabaseException::MySqlErrorCode::CreateUserError))
            throw DatabaseException(DatabaseException::RRErrorCode::CreateUserFailed, e.message(), e.userMessage());
        else
            throw;
    }
}

void UserSqlManager::viewUserDetails(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewUserDetails", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_id",
                                                               params.value("user_id", QVariant::Int)
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_id",
                                                               params.value("archived", QVariant::Bool)
                                                           }
                                                       }));

        QVariantMap userDetails;
        if (!records.isEmpty()) {
            userDetails = recordToMap(records.first());
        }

        result.setOutcome(QVariantMap {
                              { "user", userDetails },
                              { "record_count", records.count() }
                          });

    } catch (DatabaseException &) {
        throw;
    }
}

void UserSqlManager::grantPrivilege(const QString &privilege, const QString &userName, QSqlQuery &q)
{
    q.prepare(QString("GRANT %1 ON * . * TO :username@'localhost'").arg(privilege));
    q.bindValue(":username", userName);
    if (!q.exec())
        throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                QString("Failed to grant '%1' privileges to '%2'.")
                                .arg(privilege, userName));
}

void UserSqlManager::createRRUser(const QString &userName, QSqlQuery &q)
{
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    q.prepare(QString("INSERT INTO %1.user (user, photo, phone_number, email_address, active, pending, created, last_edited) "
                      "VALUES (:user, :photo, :phone_number, :email_address, :active, :pending, :created, :last_edited)")
              .arg(Config::instance().databaseName()));
    q.bindValue(":user", userName);
    q.bindValue(":photo", QVariant(QVariant::ByteArray));
    q.bindValue(":phone_number", QVariant(QVariant::String));
    q.bindValue(":email_address", QVariant(QVariant::String));
    q.bindValue(":active", false);
    q.bindValue(":pending", QVariant(QVariant::Bool));
    q.bindValue(":created", currentDateTime);
    q.bindValue(":last_edited", currentDateTime);

    if (!q.exec())
        throw DatabaseException(DatabaseException::RRErrorCode::SignUpFailure, q.lastError().text(),
                                QString("Failed to create RR user '%1'.").arg(userName));
}

QString UserSqlManager::determineRank(const QVariantList &privileges)
{
    Q_UNUSED(privileges)
    return QString();
}
