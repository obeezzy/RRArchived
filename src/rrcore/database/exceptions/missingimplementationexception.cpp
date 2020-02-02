#include "missingimplementationexception.h"

MissingImplementationException::MissingImplementationException() :
    DatabaseException(DatabaseError::QueryErrorCode::NotYetImplementedError,
                      QStringLiteral("Section not yet implemented."))
{

}

QString MissingImplementationException::toString() const
{
    return QStringLiteral("MissingImplementationException(%1)")
            .arg(message());
}
