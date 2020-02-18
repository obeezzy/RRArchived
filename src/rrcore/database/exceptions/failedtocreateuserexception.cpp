#include "failedtocreateuserexception.h"
#include <QSqlError>

FailedToCreateUserException::FailedToCreateUserException(const QString &message,
                                                         const QSqlError &error) :
    DatabaseException(DatabaseError::QueryErrorCode::CreateUserFailed,
                      message.isEmpty() ? QStringLiteral("Failed to create user.")
                                        : message,
                      error)
{

}

QString FailedToCreateUserException::toString() const
{
    return QStringLiteral("FailedToCreateUserException(%1, database=%2, code=%3)")
            .arg(message(),
                 sqlError().databaseText(),
                 sqlError().nativeErrorCode());
}
