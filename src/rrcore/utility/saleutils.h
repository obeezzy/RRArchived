#ifndef SALEUTILS_H
#define SALEUTILS_H

#include <QString>
#include <QList>
#include <initializer_list>

struct SalePayment {
    enum class PaymentMethod {
        Cash,
        DebitCard,
        CreditCard
    };

    double amount;
    PaymentMethod method;
    QString note;
    QString currency;

    QString paymentMethodAsString() const {
        switch (method) {
        case PaymentMethod::Cash:
            return QStringLiteral("cash");
        case PaymentMethod::DebitCard:
            return QStringLiteral("debit_card");
        case PaymentMethod::CreditCard:
            return QStringLiteral("credit_card");
        }

        return QString();
    }

    QVariantMap toVariantMap() const {
        return {
            { "amount", amount },
            { "method", paymentMethodAsString(), },
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

#endif // SALEUTILS_H
