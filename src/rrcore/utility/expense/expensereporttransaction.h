#ifndef EXPENSEREPORTTRANSACTION_H
#define EXPENSEREPORTTRANSACTION_H

#include "utility/common/client.h"
#include <QString>
#include <QVariantList>

namespace Utility {
struct ExpenseReportTransaction
{
    Client client;
    QString purpose;
    double amount {0.0};

    explicit ExpenseReportTransaction() = default;
    explicit ExpenseReportTransaction(const QVariantMap &map);

    QVariantMap toVariantMap() const;
};

class ExpenseReportTransactionList : public QList<ExpenseReportTransaction>
{
public:
    explicit ExpenseReportTransactionList() = default;
    ExpenseReportTransactionList(std::initializer_list<ExpenseReportTransaction> transactions) :
        QList<ExpenseReportTransaction>(transactions) { }
    explicit ExpenseReportTransactionList(const QVariantList &list) :
        QList<ExpenseReportTransaction>() {
        for (const auto &variant : list)
            append(ExpenseReportTransaction{variant.toMap()});
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::ExpenseReportTransaction, Q_PRIMITIVE_TYPE);

#endif // EXPENSEREPORTTRANSACTION_H
