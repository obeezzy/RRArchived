#ifndef SALEPAYMENT_H
#define SALEPAYMENT_H

#include "utility/commonutils.h"
#include <QString>
#include <QList>
#include <initializer_list>

struct SalePayment {
    qreal amount;
    Utility::PaymentMethod method;
    QString note;
    QString currency;

    QString paymentMethodAsString() const {
        switch (method) {
        case Utility::PaymentMethod::Cash:
            return QStringLiteral("cash");
        case Utility::PaymentMethod::DebitCard:
            return QStringLiteral("debit_card");
        case Utility::PaymentMethod::CreditCard:
            return QStringLiteral("credit_card");
        }

        return QString();
    }

    QVariantMap toVariantMap() const {
        return {
            { "amount", amount },
            { "payment_method", paymentMethodAsString(), },
            { "note", note }
        };
    }
}; Q_DECLARE_TYPEINFO(SalePayment, Q_PRIMITIVE_TYPE);

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

#endif // SALEPAYMENT_H
