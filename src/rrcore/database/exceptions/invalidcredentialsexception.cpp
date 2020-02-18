#include "invalidcredentialsexception.h"
#include <QSqlError>

InvalidCredentialsException::InvalidCredentialsException(const QString &message,
                                                         const QSqlError &error) :
    DatabaseException(DatabaseError::QueryErrorCode::InvalidCredentialsError,
                      message.isEmpty() ? QStringLiteral("The credentials provided are incorrect.")
                                        : message,
                      error.databaseText()),
    m_code(error.nativeErrorCode().toInt())
{}

QString InvalidCredentialsException::toString() const
{
    return QStringLiteral("IncorrectCredentialsException(%1, database=%2, code=%3)")
            .arg(message(), userMessage(), QString::number(m_code));
}
