#include "invalidcredentialsexception.h"

InvalidCredentialsException::InvalidCredentialsException(const QString& message,
                                                         const QSqlError& error)
    : DatabaseException(
          DatabaseError::QueryErrorCode::InvalidCredentialsError,
          message.isEmpty()
              ? QStringLiteral("The credentials provided are incorrect.")
              : message,
          error)
{}

QString InvalidCredentialsException::toString() const
{
    return QStringLiteral(
               "IncorrectCredentialsException(%1, database=%2, code=%3)")
        .arg(message(), sqlError().databaseText(),
             sqlError().nativeErrorCode());
}
