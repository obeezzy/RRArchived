#include "salepayment.h"

using namespace Utility::Sales;

SalePayment::SalePayment(double amount, const Utility::PaymentMethod& method,
                         const Utility::Note& note)
    : amount(amount),
      method(method),
      note(note)
{}

SalePayment::SalePayment(const QVariantMap& map)
    : amount(map.value("amount").toDouble()),
      method(map.value("payment_method").toString()),
      note(Note{map})
{}

QVariantMap SalePayment::toVariantMap() const
{
    return {{"amount", amount.toDouble()},
            {"payment_method", method.toString()},
            {"note_id", note.id},
            {"note", note.note}};
}

int SalePaymentList::count(const Utility::PaymentMethod& method) const
{
    int counted = 0;
    for (const auto& payment : *this)
        if (payment.method == method)
            ++counted;
    return counted;
}

Utility::Money SalePaymentList::total() const
{
    Utility::Money totalAmount;
    for (const auto& payment : *this)
        totalAmount += payment.amount;
    return Utility::Money{totalAmount};
}
