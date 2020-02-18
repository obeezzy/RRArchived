#include "procedurecallfailedexception.h"

ProcedureCallFailedException::ProcedureCallFailedException(const QString &message,
                                                           const QSqlError &error) :
    DatabaseException(DatabaseError::QueryErrorCode::ProcedureFailed,
                      message,
                      error)
{}

QString ProcedureCallFailedException::toString() const
{
    if (!sqlError().databaseText().isEmpty())
        return QStringLiteral("ProcedureCallFailedException(%1, database=%2)")
                .arg(message(), sqlError().databaseText());

    return QStringLiteral("ProcedureCallFailedException(%1)")
            .arg(message());
}
