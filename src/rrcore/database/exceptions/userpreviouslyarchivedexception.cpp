#include "userpreviouslyarchivedexception.h"

UserPreviouslyArchivedException::UserPreviouslyArchivedException(const QString &message) :
    DatabaseException(DatabaseError::QueryErrorCode::UserPreviouslyArchived,
                      message.isEmpty() ? QStringLiteral("User previously archived.")
                                        : message)
{

}

QString UserPreviouslyArchivedException::toString() const
{
    return QStringLiteral("UserPreviouslyArchivedException(%1)")
            .arg(message());
}

