#ifndef ADDINCOMETRANSACTION_H
#define ADDINCOMETRANSACTION_H

#include "incomeexecutor.h"

class Client;
namespace Utility {
    enum class PaymentMethod;
}

namespace IncomeQuery {
class AddIncomeTransaction : public IncomeExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = QStringLiteral("add_income_transaction");

    explicit AddIncomeTransaction(const Client &client,
                                  const QString &purpose,
                                  qreal amount,
                                  const Utility::PaymentMethod &paymentMethod,
                                  QObject *receiver);
    QueryResult execute() override;
};
}

#endif // ADDINCOMETRANSACTION_H
