#ifndef BEGINTRANSACTIONFAILEDEXCEPTION_H
#define BEGINTRANSACTIONFAILEDEXCEPTION_H

#include "database/databaseexception.h"

class BeginTransactionFailedException : public DatabaseException
{
public:
    explicit BeginTransactionFailedException(const QSqlError& error);
    QString toString() const override;
};

#endif  // BEGINTRANSACTIONFAILEDEXCEPTION_H
