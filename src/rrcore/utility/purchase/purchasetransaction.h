#ifndef PURCHASETRANSACTION_H
#define PURCHASETRANSACTION_H

#include "purchasecartproduct.h"
#include "purchasepayment.h"
#include "utility/commonutils.h"
#include <QString>

namespace Utility {
struct PurchaseTransaction {
    qint64 id {0};
    Vendor vendor;
    qreal totalCost {0.0};
    qreal amountPaid {0.0};
    qreal balance {0.0};
    qreal discount {0.0};
    PurchaseCartProductList products;
    PurchasePaymentList payments;
    Note note;
    QDateTime dueDateTime;
    QString action;
    RecordGroup::Flags flags;
    QDateTime created;
    QDateTime lastEdited;
    User user;
    int row {-1};

    explicit PurchaseTransaction() = default;
    explicit PurchaseTransaction(qint64 id,
                                 const Vendor &vendor,
                                 qreal totalCost,
                                 qreal amountPaid,
                                 qreal balance,
                                 const RecordGroup::Flags &flags,
                                 const PurchaseCartProductList &products,
                                 const PurchasePaymentList &payments,
                                 const QDateTime &dueDateTime = QDateTime(),
                                 const QString &action = QString(),
                                 const Note &note = Note());
    explicit PurchaseTransaction(qint64 id,
                                 const Vendor &vendor,
                                 qreal totalCost,
                                 qreal amountPaid,
                                 qreal balance,
                                 const RecordGroup::Flags &flags,
                                 const PurchaseCartProductList &products,
                                 const PurchasePaymentList &payments,
                                 const Note &note = Note());
    explicit PurchaseTransaction(const QVariantMap &transaction);
    QVariantMap toVariantMap() const;
};

class PurchaseTransactionList : public QList<PurchaseTransaction> {
public:
    explicit PurchaseTransactionList() = default;
    PurchaseTransactionList(std::initializer_list<PurchaseTransaction> transactions) :
        QList<PurchaseTransaction>(transactions) { }
    explicit PurchaseTransactionList(const QVariantList &list) :
        QList<PurchaseTransaction>() {
        for (const auto &variant : list)
            append(PurchaseTransaction{ variant.toMap() });
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
} Q_DECLARE_TYPEINFO(Utility::PurchaseTransaction, Q_PRIMITIVE_TYPE);

#endif // PURCHASETRANSACTION_H
