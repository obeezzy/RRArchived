#include "sqlstatementfailedexception.h"

SqlStatementFailedException::SqlStatementFailedException(const QString &message,
                                                         const QString &databaseText) :
    DatabaseException(DatabaseError::QueryErrorCode::SqlStatementFailedError,
                      message,
                      databaseText)
{

}

QString SqlStatementFailedException::toString() const
{
    return QStringLiteral("SqlStatementFailedException(%1)")
            .arg(message());
}
