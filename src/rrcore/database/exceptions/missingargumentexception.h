#ifndef MISSINGARGUMENTEXCEPTION_H
#define MISSINGARGUMENTEXCEPTION_H

#include "database/databaseexception.h"

class MissingArgumentException : public DatabaseException
{
public:
    explicit MissingArgumentException(const QString& message = QString());
    QString toString() const override;
};

#endif  // MISSINGARGUMENTEXCEPTION_H
