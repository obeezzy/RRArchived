#ifndef INCOMEREPORTTRANSACTION_H
#define INCOMEREPORTTRANSACTION_H

#include "utility/common/client.h"
#include <QString>
#include <QVariantList>

namespace Utility {
struct IncomeReportTransaction
{
    Client client;
    QString purpose;
    double amount {0.0};

    explicit IncomeReportTransaction() = default;
    explicit IncomeReportTransaction(const QVariantMap &map);

    QVariantMap toVariantMap() const;
};

class IncomeReportTransactionList : public QList<IncomeReportTransaction>
{
public:
    explicit IncomeReportTransactionList() = default;
    IncomeReportTransactionList(std::initializer_list<IncomeReportTransaction> transactions) :
        QList<IncomeReportTransaction>(transactions) { }
    explicit IncomeReportTransactionList(const QVariantList &list) :
        QList<IncomeReportTransaction>() {
        for (const auto &variant : list)
            append(IncomeReportTransaction{variant.toMap()});
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::IncomeReportTransaction, Q_PRIMITIVE_TYPE);

#endif // INCOMEREPORTTRANSACTION_H
