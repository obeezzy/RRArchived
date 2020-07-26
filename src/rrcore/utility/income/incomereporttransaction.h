#ifndef INCOMEREPORTTRANSACTION_H
#define INCOMEREPORTTRANSACTION_H

#include <QString>
#include <QVariantList>
#include "utility/common/client.h"
#include "utility/common/money.h"

namespace Utility {
namespace Income {
struct IncomeReportTransaction
{
    Client client;
    QString purpose;
    Money amount;

    explicit IncomeReportTransaction() = default;
    explicit IncomeReportTransaction(const QVariantMap& map);

    QVariantMap toVariantMap() const;
};

class IncomeReportTransactionList : public QList<IncomeReportTransaction>
{
public:
    explicit IncomeReportTransactionList() = default;
    IncomeReportTransactionList(
        std::initializer_list<IncomeReportTransaction> transactions)
        : QList<IncomeReportTransaction>(transactions)
    {}
    explicit IncomeReportTransactionList(const QVariantList& list)
        : QList<IncomeReportTransaction>()
    {
        for (const auto& variant : list)
            append(IncomeReportTransaction{variant.toMap()});
    }

    QVariantList toVariantList() const
    {
        QVariantList list;
        for (const auto& transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
}  // namespace Income
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::Income::IncomeReportTransaction, Q_PRIMITIVE_TYPE);

#endif  // INCOMEREPORTTRANSACTION_H
