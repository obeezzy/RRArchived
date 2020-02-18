#include "useraccountlockedexception.h"
#include <QSqlError>

UserAccountLockedException::UserAccountLockedException(const QString &message,
                                                       const QSqlError &error) :
    DatabaseException(DatabaseError::QueryErrorCode::UserAccountIsLocked,
                      message.isEmpty() ? QStringLiteral("User account is locked.")
                                        : message,
                      error)
{

}

QString UserAccountLockedException::toString() const
{
    return QStringLiteral("UserAccountLockedException(%1, database=%2, code=%3")
            .arg(message(), sqlError().databaseText(), sqlError().nativeErrorCode());
}
