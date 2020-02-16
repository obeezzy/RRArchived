#include "procedurecallfailedexception.h"

ProcedureCallFailedException::ProcedureCallFailedException(const QString &message,
                                                           const QSqlError &sqlError) :
    DatabaseException(DatabaseError::QueryErrorCode::ProcedureFailed,
                      message,
                      sqlError.databaseText())
{}

QString ProcedureCallFailedException::toString() const
{
    if (!userMessage().isEmpty())
        return QStringLiteral("ProcedureCallFailedException(%1, database=%2)")
                .arg(message(), userMessage());

    return QStringLiteral("ProcedureCallFailedException(%1)")
            .arg(message());
}
