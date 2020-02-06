#include "noexistingrecordexception.h"

NoExistingRecordException::NoExistingRecordException(const QString &message) :
    DatabaseException(DatabaseError::QueryErrorCode::NoExistingRecordError,
                      message.isEmpty() ? QStringLiteral("No existing record.")
                                        : message)
{

}

QString NoExistingRecordException::toString() const
{
    return QStringLiteral("NoExistingRecordException(%1)")
            .arg(message());
}
