#ifndef FAILEDTOCREATEUSEREXCEPTION_H
#define FAILEDTOCREATEUSEREXCEPTION_H

#include "database/databaseexception.h"

class FailedToCreateUserException : public DatabaseException
{
public:
    explicit FailedToCreateUserException(const QString &message = QString());
    QString toString() const override;
};

#endif // FAILEDTOCREATEUSEREXCEPTION_H
