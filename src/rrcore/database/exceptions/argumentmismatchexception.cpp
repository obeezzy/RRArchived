#include "argumentmismatchexception.h"
#include "database/databaseerror.h"

ArgumentMismatchException::ArgumentMismatchException(const QString& message)
    : DatabaseException(
          DatabaseError::QueryErrorCode::ArgumentMismatch,
          message.isEmpty()
              ? QStringLiteral(
                    "Equal arguments were expected, but mismatch found.")
              : message)
{}

QString ArgumentMismatchException::toString() const
{
    return QStringLiteral("ArgumentMismatchException(%1)").arg(message());
}
