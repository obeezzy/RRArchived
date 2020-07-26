#include "userexecutor.h"
#include "config/config.h"
#include "database/databaseerror.h"
#include "database/exceptions/exceptions.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QVariant>
#include <QSqlError>

UserExecutor::UserExecutor(const QString &command,
                           const QVariantMap &params,
                           QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::QueryGroup::User, receiver)
{

}

void UserExecutor::createRRUser(const QString &userName, QSqlQuery &q)
{
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    q.prepare(QStringLiteral("INSERT INTO %1.user (user, photo, phone_number, email_address, active, pending, created, last_edited) "
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
        throw FailedToCreateUserException(QStringLiteral("Failed to create RR user '%1'.").arg(userName),
                                          q.lastError());
}

void UserExecutor::grantPrivilege(const QString &privilege, const QString &userName, QSqlQuery &q)
{
    q.prepare(QString("GRANT %1 ON * . * TO :username@'localhost'").arg(privilege));
    q.bindValue(":username", userName);
    if (!q.exec())
        throw FailedToCreateUserException(QStringLiteral("Failed to grant '%1' privileges to '%2'.")
                                          .arg(privilege, userName),
                                          q.lastError());
}
