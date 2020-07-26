#include "incorrectpasswordexception.h"

IncorrectPasswordException::IncorrectPasswordException(
    const QString& message, const QString& databaseText)
    : DatabaseException(
          DatabaseError::QueryErrorCode::IncorrectPasswordError,
          message.isEmpty()
              ? QStringLiteral("The password provided is incorrect.")
              : message,
          databaseText)
{}

QString IncorrectPasswordException::toString() const
{
    return QStringLiteral("IncorrectPasswordException(%1)").arg(message());
}
