#ifndef INVALIDDUEDATEEXCEPTION_H
#define INVALIDDUEDATEEXCEPTION_H

#include "database/databaseexception.h"

class InvalidDueDateException : public DatabaseException
{
public:
    explicit InvalidDueDateException();
    QString toString() const override;
};

#endif // INVALIDDUEDATEEXCEPTION_H
