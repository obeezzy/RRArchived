#ifndef USERACCOUNTLOCKEDEXCEPTION_H
#define USERACCOUNTLOCKEDEXCEPTION_H

#include "database/databaseexception.h"

class UserAccountLockedException : public DatabaseException
{
public:
    explicit UserAccountLockedException(const QString &message = QString());
};

#endif // USERACCOUNTLOCKEDEXCEPTION_H
