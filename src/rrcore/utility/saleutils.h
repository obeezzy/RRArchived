#ifndef SALEUTILS_H
#define SALEUTILS_H

#include <QString>

struct SalePayment {
    enum class PaymentMethod {
        Cash,
        DebitCard,
        CreditCard
    };

    double amount;
    PaymentMethod method;
    QString note;
}; Q_DECLARE_TYPEINFO(SalePayment, Q_PRIMITIVE_TYPE);


#endif // SALEUTILS_H
