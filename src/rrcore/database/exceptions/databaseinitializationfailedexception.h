#ifndef DATABASEINITIALIZATIONFAILEDEXCEPTION_H
#define DATABASEINITIALIZATIONFAILEDEXCEPTION_H

#include "database/databaseexception.h"

class DatabaseInitializationFailedException : public DatabaseException
{
public:
    explicit DatabaseInitializationFailedException(
        const QString& message, const QSqlError& error = QSqlError());
    QString toString() const override;
};

#endif  // DATABASEINITIALIZATIONFAILEDEXCEPTION_H
