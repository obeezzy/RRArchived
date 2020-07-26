#include "invalidduedateexception.h"
#include "database/databaseerror.h"

InvalidDueDateException::InvalidDueDateException()
    : DatabaseException(
          DatabaseError::QueryErrorCode::InvalidDueDate,
          QStringLiteral(
              "Due date is earlier than the current date or invalid."))
{}

QString InvalidDueDateException::toString() const
{
    return QStringLiteral("InvalidDueDateException(%1)").arg(message());
}
