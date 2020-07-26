#ifndef PURCHASEPAYMENT_H
#define PURCHASEPAYMENT_H

#include <QList>
#include <QString>
#include "utility/common/money.h"
#include "utility/common/note.h"
#include "utility/common/paymentmethod.h"

namespace Utility {
namespace Purchase {
struct PurchasePayment
{
    Money amount;
    Utility::PaymentMethod method;
    Note note;

    explicit PurchasePayment(const Money& amount,
                             const Utility::PaymentMethod& method,
                             const Note& note = Note());

    explicit PurchasePayment(const QVariantMap& map);
    QVariantMap toVariantMap() const;
};

class PurchasePaymentList : public QList<PurchasePayment>
{
public:
    inline QVariantList toVariantList() const
    {
        QVariantList list;
        for (const auto& payment : *this)
            list.append(payment.toVariantMap());
        return list;
    }

    int count(const PaymentMethod& method = PaymentMethod()) const;
    Money total() const;
};
}  // namespace Purchase
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::Purchase::PurchasePayment, Q_PRIMITIVE_TYPE);

#endif  // PURCHASEPAYMENT_H
