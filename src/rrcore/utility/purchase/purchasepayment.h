#ifndef PURCHASEPAYMENT_H
#define PURCHASEPAYMENT_H

#include "utility/commonutils.h"
#include <QString>
#include <QList>

namespace Utility {
struct PurchasePayment {
    qreal amount {0.0};
    Utility::PaymentMethod method;
    Note note;
    QString currency;

    explicit PurchasePayment(qreal amount,
                             const Utility::PaymentMethod &method,
                             const Note &note,
                             const QString &currency);

    QVariantMap toVariantMap() const;
};

class PurchasePaymentList : public QList<PurchasePayment> {
public:
    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &payment : *this)
            list.append(payment.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::PurchasePayment, Q_PRIMITIVE_TYPE);

#endif // PURCHASEPAYMENT_H
