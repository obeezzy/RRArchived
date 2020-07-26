#ifndef ADDINCOMETRANSACTION_H
#define ADDINCOMETRANSACTION_H

#include "incomeexecutor.h"

namespace Utility {
namespace Income {
class IncomeTransaction;
}
}

namespace Query {
    namespace Income {
    class AddIncomeTransaction : public IncomeExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("add_income_transaction");

        explicit AddIncomeTransaction(const Utility::Income::IncomeTransaction &transaction,
                                      QObject *receiver);
        QueryResult execute() override;
    };
}
}

#endif // ADDINCOMETRANSACTION_H
