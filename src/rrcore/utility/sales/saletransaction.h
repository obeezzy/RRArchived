#ifndef SALETRANSACTION_H
#define SALETRANSACTION_H

#include "utility/commonutils.h"
#include "salecartproduct.h"
#include "salepayment.h"
#include <QString>
#include <QDateTime>

namespace Utility {
struct SaleTransaction
{
    int id {-1};
    Customer customer;
    qreal totalCost {0.0};
    qreal amountPaid {0.0};
    qreal balance {0.0};
    qreal discount {0.0};
    SalePaymentList payments;
    SaleCartProductList products;
    RecordGroup::Flags flags;
    QDateTime dueDateTime;
    QString action;
    Note note;
    QDateTime created;
    QDateTime lastEdited;
    User user;
    int row {-1};

    explicit SaleTransaction() = default;
    explicit SaleTransaction(const QVariantMap &map);
    explicit SaleTransaction(qint64 id);
    explicit SaleTransaction(qint64 id,
                             const Customer &customer,
                             qreal totalCost,
                             qreal amountPaid,
                             qreal balance,
                             const RecordGroup::Flags &flags,
                             const QDateTime &dueDateTime,
                             const QString &action,
                             const Note &note,
                             const SalePaymentList &payments,
                             const SaleCartProductList &products);
    explicit SaleTransaction(qint64 id,
                             const Customer &customer,
                             qreal totalCost,
                             qreal amountPaid,
                             qreal balance,
                             const RecordGroup::Flags &flags,
                             const Note &note);

    QVariantMap toVariantMap() const;
};

class SaleTransactionList : public QList<SaleTransaction>
{
public:
    explicit SaleTransactionList() = default;
    SaleTransactionList(std::initializer_list<SaleTransaction> transactions) :
        QList<SaleTransaction>(transactions) { }
    explicit SaleTransactionList(const QVariantList &list) :
        QList<SaleTransaction>() {
        for (const auto &variant : list)
            append(SaleTransaction{variant.toMap()});
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
} Q_DECLARE_TYPEINFO(Utility::SaleTransaction, Q_PRIMITIVE_TYPE);

#endif // SALETRANSACTION_H
