#ifndef EXPENSETRANSACTION_H
#define EXPENSETRANSACTION_H

#include "utility/common/client.h"
#include "utility/common/paymentmethod.h"
#include "singletons/settings.h"
#include <QString>
#include <QVariantList>

namespace Utility {
struct ExpenseTransaction
{
    int id {-1};
    Client client;
    QString purpose;
    qreal amount {0.0};
    PaymentMethod paymentMethod;
    QString currency {Settings::DEFAULT_CURRENCY};

    explicit ExpenseTransaction() = default;
    explicit ExpenseTransaction(const QVariantMap &map);

    QVariantMap toVariantMap() const;
};

class ExpenseTransactionList : public QList<ExpenseTransaction>
{
public:
    explicit ExpenseTransactionList() = default;
    ExpenseTransactionList(std::initializer_list<ExpenseTransaction> transactions) :
        QList<ExpenseTransaction>(transactions) { }
    explicit ExpenseTransactionList(const QVariantList &list) :
        QList<ExpenseTransaction>() {
        for (const auto &variant : list)
            append(ExpenseTransaction{ variant.toMap() });
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::ExpenseTransaction, Q_PRIMITIVE_TYPE);

#endif // EXPENSETRANSACTION_H
