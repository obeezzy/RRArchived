#ifndef INCOMETRANSACTION_H
#define INCOMETRANSACTION_H

#include "singletons/settings.h"
#include "utility/common/client.h"
#include "utility/common/money.h"
#include "utility/common/paymentmethod.h"
#include <QString>
#include <QVariantList>

namespace Utility {
struct IncomeTransaction
{
    int id {0};
    Client client;
    QString purpose;
    Money amount;
    PaymentMethod paymentMethod;

    explicit IncomeTransaction() = default;
    explicit IncomeTransaction(const QVariantMap &map);

    QVariantMap toVariantMap() const;

    friend QDebug operator<<(QDebug debug, const IncomeTransaction &transaction)
    {
        debug.nospace() << "IncomeTransaction("
                        << "id=" << transaction.id
                        << ", client=" << transaction.client
                        << ", purpose=" << transaction.purpose
                        << ", amount=" << transaction.amount
                        << ", paymentMethod=" << transaction.paymentMethod
                        << ")";

        return debug.nospace();
    }
};

class IncomeTransactionList : public QList<IncomeTransaction>
{
public:
    explicit IncomeTransactionList() = default;
    IncomeTransactionList(std::initializer_list<IncomeTransaction> transactions) :
        QList<IncomeTransaction>(transactions) { }
    explicit IncomeTransactionList(const QVariantList &list) :
        QList<IncomeTransaction>() {
        for (const auto &variant : list)
            append(IncomeTransaction{ variant.toMap() });
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::IncomeTransaction, Q_PRIMITIVE_TYPE);

#endif // INCOMETRANSACTION_H
