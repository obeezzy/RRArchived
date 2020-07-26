#include "unexpectedresultexception.h"
#include "database/databaseerror.h"

UnexpectedResultException::UnexpectedResultException(const QString& message)
    : DatabaseException(DatabaseError::QueryErrorCode::UnexpectedResultError,
                        message)
{}

QString UnexpectedResultException::toString() const
{
    return QStringLiteral("UnexpectedResultException(%1)").arg(message());
}
