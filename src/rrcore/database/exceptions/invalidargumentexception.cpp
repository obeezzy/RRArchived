#include "invalidargumentexception.h"
#include "database/databaseerror.h"

InvalidArgumentException::InvalidArgumentException(const QString &message) :
    DatabaseException(DatabaseError::QueryErrorCode::InvalidArguments,
                      message.isEmpty() ? QStringLiteral("Invalid arguments set.") : QString())
{

}

QString InvalidArgumentException::toString() const
{
    return QStringLiteral("InvalidArgumentException(%1)")
            .arg(message());
}
