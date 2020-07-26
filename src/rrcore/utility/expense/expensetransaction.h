#ifndef EXPENSETRANSACTION_H
#define EXPENSETRANSACTION_H

#include <QString>
#include <QVariantList>
#include "singletons/settings.h"
#include "utility/common/client.h"
#include "utility/common/money.h"
#include "utility/common/paymentmethod.h"

namespace Utility {
namespace Expense {
struct ExpenseTransaction
{
    int id{0};
    Client client;
    QString purpose;
    Money amount;
    PaymentMethod paymentMethod;

    explicit ExpenseTransaction() = default;
    explicit ExpenseTransaction(const QVariantMap& map);

    QVariantMap toVariantMap() const;

    friend QDebug operator<<(QDebug debug,
                             const ExpenseTransaction& transaction)
    {
        debug.nospace() << "ExpenseTransaction("
                        << "id=" << transaction.id
                        << ", client=" << transaction.client
                        << ", purpose=" << transaction.purpose
                        << ", amount=" << transaction.amount
                        << ", paymentMethod=" << transaction.paymentMethod
                        << ")";

        return debug.nospace();
    }
};

class ExpenseTransactionList : public QList<ExpenseTransaction>
{
public:
    explicit ExpenseTransactionList() = default;
    ExpenseTransactionList(
        std::initializer_list<ExpenseTransaction> transactions)
        : QList<ExpenseTransaction>(transactions)
    {}
    explicit ExpenseTransactionList(const QVariantList& list)
        : QList<ExpenseTransaction>()
    {
        for (const auto& variant : list)
            append(ExpenseTransaction{variant.toMap()});
    }

    QVariantList toVariantList() const
    {
        QVariantList list;
        for (const auto& transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
}  // namespace Expense
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::Expense::ExpenseTransaction, Q_PRIMITIVE_TYPE);

#endif  // EXPENSETRANSACTION_H
