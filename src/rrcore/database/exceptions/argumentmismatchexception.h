#ifndef ARGUMENTMISMATCHEXCEPTION_H
#define ARGUMENTMISMATCHEXCEPTION_H

#include "database/databaseexception.h"

class ArgumentMismatchException : public DatabaseException
{
public:
    explicit ArgumentMismatchException(const QString& message = QString());
    QString toString() const override;
};

#endif  // ARGUMENTMISMATCHEXCEPTION_H
