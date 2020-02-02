#ifndef MISSINGIMPLEMENTATIONEXCEPTION_H
#define MISSINGIMPLEMENTATIONEXCEPTION_H

#include "database/databaseexception.h"

class MissingImplementationException : public DatabaseException
{
public:
    explicit MissingImplementationException();
    QString toString() const override;
};

#endif // MISSINGIMPLEMENTATIONEXCEPTION_H
