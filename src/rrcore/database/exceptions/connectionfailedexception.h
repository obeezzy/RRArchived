#ifndef CONNECTIONFAILEDEXCEPTION_H
#define CONNECTIONFAILEDEXCEPTION_H

#include "database/databaseexception.h"

class QSqlError;

class ConnectionFailedException : public DatabaseException
{
public:
    explicit ConnectionFailedException(const QString &message,
                                       const QSqlError &error);
    QString toString() const override;
};

#endif // CONNECTIONFAILEDEXCEPTION_H
