#ifndef SQLSTATEMENTFAILEDEXCEPTION_H
#define SQLSTATEMENTFAILEDEXCEPTION_H

#include "database/databaseexception.h"

class SqlStatementFailedException : public DatabaseException
{
public:
    explicit SqlStatementFailedException(const QString& statement = QString(),
                                         const QSqlError& error = QSqlError());
    QString toString() const override;
};

#endif  // SQLSTATEMENTFAILEDEXCEPTION_H
