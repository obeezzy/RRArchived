#ifndef INCOMETRANSACTION_H
#define INCOMETRANSACTION_H

#include "utility/common/client.h"
#include "utility/common/paymentmethod.h"
#include <QString>
#include <QVariantList>

namespace Utility {
struct IncomeTransaction
{
    int id {-1};
    Client client;
    QString purpose;
    qreal amount {0.0};
    PaymentMethod paymentMethod;

    explicit IncomeTransaction() = default;
    explicit IncomeTransaction(const QVariantMap &map);

    QVariantMap toVariantMap() const;
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
