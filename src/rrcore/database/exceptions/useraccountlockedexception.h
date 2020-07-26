#ifndef USERACCOUNTLOCKEDEXCEPTION_H
#define USERACCOUNTLOCKEDEXCEPTION_H

#include "database/databaseexception.h"

class UserAccountLockedException : public DatabaseException
{
public:
    explicit UserAccountLockedException(const QString& message,
                                        const QSqlError& error);
    QString toString() const override;
};

#endif  // USERACCOUNTLOCKEDEXCEPTION_H
