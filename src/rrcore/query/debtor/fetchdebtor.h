#ifndef FETCHDEBTOR_H
#define FETCHDEBTOR_H

#include "debtorexecutor.h"

namespace Query {
    namespace Debtor {
    class FetchDebtor : public DebtorExecutor
    {
    public:
        static inline const QString COMMAND = QStringLiteral("fetch_debtor");

        explicit FetchDebtor(int debtorId,
                             QObject *receiver);
        QueryResult execute() override;
    };
}
}

#endif // FETCHDEBTOR_H
