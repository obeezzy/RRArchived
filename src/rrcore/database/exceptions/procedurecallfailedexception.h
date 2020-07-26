#ifndef PROCEDURECALLFAILEDEXCEPTION_H
#define PROCEDURECALLFAILEDEXCEPTION_H

#include "database/databaseexception.h"

class ProcedureCallFailedException : public DatabaseException
{
public:
    explicit ProcedureCallFailedException(const QString& message,
                                          const QSqlError& error = QSqlError());
    QString toString() const override;
};

#endif  // PROCEDURECALLFAILEDEXCEPTION_H
