#include "missingargumentexception.h"
#include "database/databaseerror.h"
#include "database/exceptions/missingargumentexception.h"

MissingArgumentException::MissingArgumentException(const QString &message) :
    DatabaseException(DatabaseError::QueryErrorCode::MissingArguments,
                      message.isEmpty() ? QStringLiteral("Arguments missing.")
                                        : message)
{

}

QString MissingArgumentException::toString() const
{
    return QStringLiteral("MissingArgumentException(%1)")
            .arg(message());
}
