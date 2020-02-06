#ifndef NOEXISTINGRECORDEXCEPTION_H
#define NOEXISTINGRECORDEXCEPTION_H

#include "database/databaseexception.h"

class NoExistingRecordException : public DatabaseException
{
public:
    explicit NoExistingRecordException(const QString &message);
    QString toString() const override;
};

#endif // NOEXISTINGRECORDEXCEPTION_H
