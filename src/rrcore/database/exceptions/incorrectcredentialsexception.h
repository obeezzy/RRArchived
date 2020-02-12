#ifndef INCORRECTCREDENTIALSEXCEPTION_H
#define INCORRECTCREDENTIALSEXCEPTION_H

#include "database/databaseexception.h"

class IncorrectCredentialsException : public DatabaseException
{
public:
    explicit IncorrectCredentialsException(const QString &message = QString());
    QString toString() const override;
};

#endif // INCORRECTCREDENTIALSEXCEPTION_H
