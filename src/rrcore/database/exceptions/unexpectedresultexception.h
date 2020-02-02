#ifndef UNEXPECTEDRESULTEXCEPTION_H
#define UNEXPECTEDRESULTEXCEPTION_H

#include "database/databaseexception.h"

class UnexpectedResultException : public DatabaseException
{
public:
    explicit UnexpectedResultException(const QString &message);
    QString toString() const override;
};

#endif // UNEXPECTEDRESULTEXCEPTION_H
