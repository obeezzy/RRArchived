#ifndef PROCEDURECALLFAILEDEXCEPTION_H
#define PROCEDURECALLFAILEDEXCEPTION_H

#include "database/databaseexception.h"
#include <QSqlError>

class ProcedureCallFailedException : public DatabaseException
{
public:
    explicit ProcedureCallFailedException(const QString &message,
                                          const QSqlError &sqlError = QSqlError());
    QString toString() const override;
};

#endif // PROCEDURECALLFAILEDEXCEPTION_H
