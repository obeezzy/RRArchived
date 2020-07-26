#include "databaseinitializationfailedexception.h"
#include <QSqlError>

DatabaseInitializationFailedException::DatabaseInitializationFailedException(
    const QString& message, const QSqlError& error)
    : DatabaseException(
          DatabaseError::QueryErrorCode::DatabaseInitializationFailed, message,
          error)
{}

QString DatabaseInitializationFailedException::toString() const
{
    return QStringLiteral(
               "DatabaseInitializationFailedException(%1, database=%2)")
        .arg(message(), sqlError().databaseText());
}
