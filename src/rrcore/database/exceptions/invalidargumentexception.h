#ifndef INVALIDARGUMENTEXCEPTION_H
#define INVALIDARGUMENTEXCEPTION_H

#include "database/databaseexception.h"

class InvalidArgumentException : public DatabaseException
{
public:
    explicit InvalidArgumentException(const QString& message = QString());
    QString toString() const override;
};

#endif  // INVALIDARGUMENTEXCEPTION_H
