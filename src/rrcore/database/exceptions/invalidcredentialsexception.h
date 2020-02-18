#ifndef INVALIDCREDENTIALSEXCEPTION_H
#define INVALIDCREDENTIALSEXCEPTION_H

#include "database/databaseexception.h"

class InvalidCredentialsException : public DatabaseException
{
public:
    explicit InvalidCredentialsException(const QString &message,
                                         const QSqlError &error);
    QString toString() const override;
};

#endif // INVALIDCREDENTIALSEXCEPTION_H
