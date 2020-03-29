#include "purchasepayment.h"

using namespace Utility;

PurchasePayment::PurchasePayment(const Money &amount,
                                 const PaymentMethod &method,
                                 const Note &note) :
    amount(amount),
    method(method),
    note(note)
{}

PurchasePayment::PurchasePayment(const QVariantMap &map) :
    amount(map.value("amount").toDouble()),
    method(map.value("payment_method").toString()),
    note(Note{ map })
{}

QVariantMap PurchasePayment::toVariantMap() const
{
    return {
        { "amount", amount.toDouble() },
        { "payment_method", method.toString() },
        { "note", note.note },
        { "currency", amount.currency().isoCode() }
    };
}

int PurchasePaymentList::count(const PaymentMethod &method) const
{
    int counted = 0;
    for (const auto &payment : *this)
        if (payment.method == method)
            ++counted;
    return counted;
}

Money PurchasePaymentList::total() const
{
    Money totalAmount;
    for (const auto &payment : *this)
        totalAmount += payment.amount;
    return Money{ totalAmount };
}
