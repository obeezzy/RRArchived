#include "duplicateentryexception.h"
#include "database/databaseerror.h"

DuplicateEntryException::DuplicateEntryException(const QString& message)
    : DatabaseException(
          DatabaseError::QueryErrorCode::DuplicateEntryFailure,
          message.isEmpty() ? QStringLiteral("Entry already exists.") : message)
{}

QString DuplicateEntryException::toString() const
{
    return QStringLiteral("DuplicateEntryException(%1").arg(message());
}
