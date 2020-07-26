#ifndef SALEREPORTTRANSACTION_H
#define SALEREPORTTRANSACTION_H

#include "utility/stock/product.h"
#include "utility/stock/productcategory.h"
#include <QVariantList>
#include <initializer_list>

namespace Utility {
    namespace Sales {
    struct SaleReportTransaction
    {
        Stock::ProductCategory category;
        Stock::Product product;
        Stock::ProductQuantity quantitySold;
        Money totalRevenue;

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
}
}
Q_DECLARE_TYPEINFO(Utility::Sales::SaleReportTransaction, Q_PRIMITIVE_TYPE);

#endif // SALEREPORTTRANSACTION_H
