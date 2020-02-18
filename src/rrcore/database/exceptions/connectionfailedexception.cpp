#include "connectionfailedexception.h"
#include <QSqlError>

ConnectionFailedException::ConnectionFailedException(const QString &message,
                                                     const QSqlError &error) :
    DatabaseException(DatabaseError::QueryErrorCode::ConnectionFailedError,
                      message,
                      error.databaseText())
{}

QString ConnectionFailedException::toString() const
{
    return QStringLiteral("ConnectionFailedException(%1, database=%2)")
            .arg(message(), userMessage());
}

