#include "failedtocreateuserexception.h"

FailedToCreateUserException::FailedToCreateUserException(const QString &message) :
    DatabaseException(DatabaseError::QueryErrorCode::CreateUserFailed,
                      message.isEmpty() ? QStringLiteral("Failed to create user.")
                                        : message)
{

}

QString FailedToCreateUserException::toString() const
{
    return QStringLiteral("FailedToCreateUserException(%1)")
            .arg(message());
}
