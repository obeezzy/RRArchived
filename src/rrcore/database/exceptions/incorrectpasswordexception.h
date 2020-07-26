#ifndef INCORRECTPASSWORDEXCEPTION_H
#define INCORRECTPASSWORDEXCEPTION_H

#include "database/databaseexception.h"

class IncorrectPasswordException : public DatabaseException
{
public:
    explicit IncorrectPasswordException(
        const QString& message = QString(),
        const QString& databaseText = QString());
    QString toString() const override;
};

#endif  // INCORRECTPASSWORDEXCEPTION_H
