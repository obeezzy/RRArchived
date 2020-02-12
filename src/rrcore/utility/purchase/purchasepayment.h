#ifndef PURCHASEPAYMENT_H
#define PURCHASEPAYMENT_H

#include "utility/commonutils.h"
#include <QString>
#include <QList>
#include <QDateTime>

struct PurchasePayment {
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
}; Q_DECLARE_TYPEINFO(PurchasePayment, Q_PRIMITIVE_TYPE);

class PurchasePaymentList : public QList<PurchasePayment> {
public:
    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &payment : *this)
            list.append(payment.toVariantMap());
        return list;
    }
};

#endif // PURCHASEPAYMENT_H
