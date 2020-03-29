#include "amountoverpaidexception.h"
#include "database/databaseerror.h"

AmountOverpaidException::AmountOverpaidException(double amount) :
    DatabaseException(DatabaseError::QueryErrorCode::AmountOverpaid,
                      QStringLiteral("Total amount paid is greater than total debt by %1.").arg(qAbs(amount)))
{

}

QString AmountOverpaidException::toString() const
{
    return QStringLiteral("AmountOverpaidException(%1)")
            .arg(message());
}
