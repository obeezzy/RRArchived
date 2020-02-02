#ifndef DUPLICATEENTRYEXCEPTION_H
#define DUPLICATEENTRYEXCEPTION_H

#include "database/databaseexception.h"

class DuplicateEntryException : public DatabaseException
{
public:
    explicit DuplicateEntryException(const QString &message = QString());
    QString toString() const override;
};

#endif // DUPLICATEENTRYEXCEPTION_H
