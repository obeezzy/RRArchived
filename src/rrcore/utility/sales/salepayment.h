#ifndef SALEPAYMENT_H
#define SALEPAYMENT_H

#include "utility/commonutils.h"
#include <QString>

namespace Utility {
struct SalePayment {
    qreal amount {0.0};
    Utility::PaymentMethod method;
    Note note;
    QString currency;

    explicit SalePayment(qreal amount,
                         const Utility::PaymentMethod &method,
                         const Note &note,
                         const QString &currency);
    QVariantMap toVariantMap() const;
};

class SalePaymentList : public QList<SalePayment> {
public:
    explicit SalePaymentList() { }
    explicit SalePaymentList(std::initializer_list<SalePayment> paymentList) :
        QList<SalePayment>(paymentList) {}

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
