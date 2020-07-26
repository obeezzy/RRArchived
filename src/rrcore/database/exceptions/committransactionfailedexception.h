#ifndef COMMITTRANSACTIONFAILEDEXCEPTION_H
#define COMMITTRANSACTIONFAILEDEXCEPTION_H

#include "database/databaseexception.h"

class CommitTransactionFailedException : public DatabaseException
{
public:
    explicit CommitTransactionFailedException(const QSqlError& error);
    QString toString() const override;
};

#endif  // COMMITTRANSACTIONFAILEDEXCEPTION_H
