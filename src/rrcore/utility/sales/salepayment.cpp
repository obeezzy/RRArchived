#include "salepayment.h"

using namespace Utility;

SalePayment::SalePayment(double amount,
                         const Utility::PaymentMethod &method,
                         const Note &note) :
    amount(amount),
    method(method),
    note(note)
{}

SalePayment::SalePayment(const QVariantMap &map) :
    amount(map.value("amount").toDouble()),
    method(map.value("payment_method").toString()),
    note(Note{ map })
{}

QVariantMap SalePayment::toVariantMap() const
{
    return {
        { "amount", amount.toDouble() },
        { "payment_method", method.toString()},
        { "note_id", note.id },
        { "note", note.note }
    };
}

int SalePaymentList::count(const PaymentMethod &method) const {
    int counted = 0;
    for (const auto &payment : *this)
        if (payment.method == method)
            ++counted;
    return counted;
}

Money SalePaymentList::total() const
{
    Money totalAmount;
    for (const auto &payment : *this)
        totalAmount += payment.amount;
    return Money{ totalAmount };
}
