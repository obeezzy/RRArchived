#include "begintransactionfailedexception.h"
#include <QSqlError>

BeginTransactionFailedException::BeginTransactionFailedException(
    const QSqlError& error)
    : DatabaseException(DatabaseError::QueryErrorCode::BeginTransactionFailed,
                        QStringLiteral("Failed to begin transation."), error)
{}

QString BeginTransactionFailedException::toString() const
{
    return QStringLiteral("BeginTransactionFailedException(%1, database=%2)")
        .arg(message(), sqlError().databaseText());
}
