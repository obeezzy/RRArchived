#ifndef SQLSTATEMENTFAILEDEXCEPTION_H
#define SQLSTATEMENTFAILEDEXCEPTION_H

#include "database/databaseexception.h"

class SqlStatementFailedException : public DatabaseException
{
public:
    explicit SqlStatementFailedException(const QString &message = QString(),
                                         const QString &databaseText = QString());
    QString toString() const override;
};

#endif // SQLSTATEMENTFAILEDEXCEPTION_H
