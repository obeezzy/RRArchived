#include "missingcommandexception.h"

MissingCommandException::MissingCommandException(const QString& command)
    : DatabaseException(
          DatabaseError::QueryErrorCode::MissingCommandError,
          QStringLiteral("Could not find command '%1'.").arg(command))
{}

QString MissingCommandException::toString() const
{
    return QStringLiteral("MissingCommandException(%1)").arg(message());
}
