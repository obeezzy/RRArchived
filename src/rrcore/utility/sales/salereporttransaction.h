#ifndef SALEREPORTTRANSACTION_H
#define SALEREPORTTRANSACTION_H

#include "utility/stock/stockproductcategory.h"
#include "utility/stock/stockproduct.h"
#include <initializer_list>
#include <QVariantList>

namespace Utility {
struct SaleReportTransaction
{
    StockProductCategory category;
    StockProduct product;
    qreal quantitySold {0.0};
    qreal totalAmount {0.0};

    explicit SaleReportTransaction() = default;
    explicit SaleReportTransaction(const QVariantMap &map);

    QVariantMap toVariantMap() const;
};

class SaleReportTransactionList : public QList<SaleReportTransaction>
{
public:
    explicit SaleReportTransactionList() = default;
    SaleReportTransactionList(std::initializer_list<SaleReportTransaction> transactions) :
        QList<SaleReportTransaction>(transactions) { }
    explicit SaleReportTransactionList(const QVariantList &list) :
        QList<SaleReportTransaction>() {
        for (const auto &variant : list)
            append(SaleReportTransaction{variant.toMap()});
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
} Q_DECLARE_TYPEINFO(Utility::SaleReportTransaction, Q_PRIMITIVE_TYPE);
#endif // SALEREPORTTRANSACTION_H
