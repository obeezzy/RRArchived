#ifndef PURCHASEPAYMENT_H
#define PURCHASEPAYMENT_H

#include "utility/common/money.h"
#include "utility/common/note.h"
#include "utility/common/paymentmethod.h"
#include "singletons/settings.h"
#include <QString>
#include <QList>

namespace Utility {
struct PurchasePayment {
    Money amount;
    Utility::PaymentMethod method;
    Note note;

    explicit PurchasePayment(const Money &amount,
                             const Utility::PaymentMethod &method,
                             const Note &note = Note());

    explicit PurchasePayment(const QVariantMap &map);
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
