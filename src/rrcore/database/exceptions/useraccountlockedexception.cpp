#include "useraccountlockedexception.h"

UserAccountLockedException::UserAccountLockedException(const QString &message) :
    DatabaseException(DatabaseError::QueryErrorCode::UserAccountIsLocked,
                      message.isEmpty() ? QStringLiteral("User account is locked.")
                                        : message)
{

}
