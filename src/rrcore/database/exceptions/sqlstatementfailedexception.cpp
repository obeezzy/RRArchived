#include "sqlstatementfailedexception.h"

SqlStatementFailedException::SqlStatementFailedException(const QString &statement,
                                                         const QSqlError &error) :
    DatabaseException(DatabaseError::QueryErrorCode::SqlStatementFailedError,
                      QStringLiteral("Failed to execute query: %1").arg(statement),
                      error)
{

}

QString SqlStatementFailedException::toString() const
{
    return QStringLiteral("SqlStatementFailedException(%1, database=%2)")
            .arg(message(), sqlError().databaseText());
}
