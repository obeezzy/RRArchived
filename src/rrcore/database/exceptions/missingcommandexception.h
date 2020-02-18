#ifndef MISSINGCOMMANDEXCEPTION_H
#define MISSINGCOMMANDEXCEPTION_H

#include "database/databaseexception.h"

class MissingCommandException : public DatabaseException
{
public:
    explicit MissingCommandException(const QString &command);
    QString toString() const override;
};

#endif // MISSINGCOMMANDEXCEPTION_H
