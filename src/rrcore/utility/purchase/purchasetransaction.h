#ifndef PURCHASETRANSACTION_H
#define PURCHASETRANSACTION_H

#include "purchasecartproduct.h"
#include "purchasepayment.h"
#include "utility/commonutils.h"
#include <QString>

struct PurchaseTransaction {
    qint64 id;
    Vendor vendor;
    qreal totalCost;
    qreal amountPaid;
    qreal balance;
    qreal discount;
    PurchaseCartProductList products;
    PurchasePaymentList payments;
    Note note;
    QDateTime dueDateTime;
    QString action;
    RecordGroup::Flags flags;
    QDateTime created;
    QDateTime lastEdited;
    int userId;
    int row;

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
}; Q_DECLARE_TYPEINFO(PurchaseTransaction, Q_PRIMITIVE_TYPE);

class PurchaseTransactionList : public QList<PurchaseTransaction> {
public:
    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};

#endif // PURCHASETRANSACTION_H
