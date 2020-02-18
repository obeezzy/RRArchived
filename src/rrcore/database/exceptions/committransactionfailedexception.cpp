#include "committransactionfailedexception.h"
#include <QSqlError>

CommitTransactionFailedException::CommitTransactionFailedException(const QSqlError &error) :
    DatabaseException(DatabaseError::QueryErrorCode::CommitTransationFailed,
                      QStringLiteral("Failed to commit transaction."),
                      error)
{}

QString CommitTransactionFailedException::toString() const
{
    return QStringLiteral("CommitTransactionFailedException(%1, database=%2)")
            .arg(message(), sqlError().databaseText());
}
