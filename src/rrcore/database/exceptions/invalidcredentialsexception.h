#ifndef INVALIDCREDENTIALSEXCEPTION_H
#define INVALIDCREDENTIALSEXCEPTION_H

#include "database/databaseexception.h"

class QSqlError;

class InvalidCredentialsException : public DatabaseException
{
public:
    explicit InvalidCredentialsException(const QString &message,
                                         const QSqlError &error);
    QString toString() const override;
private:
    int m_code {-1};
};

#endif // INVALIDCREDENTIALSEXCEPTION_H
