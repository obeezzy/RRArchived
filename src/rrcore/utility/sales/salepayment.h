#ifndef SALEPAYMENT_H
#define SALEPAYMENT_H

#include "utility/common/money.h"
#include "utility/common/note.h"
#include "utility/common/paymentmethod.h"
#include "singletons/settings.h"
#include <QString>

namespace Utility {
struct SalePayment {
    Money amount;
    Utility::PaymentMethod method;
    Note note;

    explicit SalePayment(double amount,
                         const Utility::PaymentMethod &method,
                         const Note &note = Note());
    explicit SalePayment(const QVariantMap &map);
    QVariantMap toVariantMap() const;

    friend QDebug operator<<(QDebug debug, const SalePayment &payment)
    {
        debug.nospace() << "SaleTransaction("
                        << "amount=" << payment.amount
                        << ", method=" << payment.method
                        << ", note=" << payment.note
                        << ")";

        return debug.nospace();
    }
};

class SalePaymentList : public QList<SalePayment> {
public:
    explicit SalePaymentList() { }
    explicit SalePaymentList(std::initializer_list<SalePayment> paymentList) :
        QList<SalePayment>(paymentList) {}
    explicit SalePaymentList(const QVariantList &list) :
        QList<SalePayment>() {
        for (const auto &variant : list)
            append(SalePayment{variant.toMap()});
    }
    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &payment : *this)
            list.append(payment.toVariantMap());

        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::SalePayment, Q_PRIMITIVE_TYPE);

#endif // SALEPAYMENT_H
