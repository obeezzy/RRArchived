#include "incorrectcredentialsexception.h"

IncorrectCredentialsException::IncorrectCredentialsException(const QString &message) :
    DatabaseException(DatabaseError::QueryErrorCode::NoCommand,
                      message.isEmpty() ? QStringLiteral("The credentials provided are incorrect.")
                                        : message)
{

}

QString IncorrectCredentialsException::toString() const
{
    return QStringLiteral("IncorrectCredentialException(%1)")
            .arg(message());
}
