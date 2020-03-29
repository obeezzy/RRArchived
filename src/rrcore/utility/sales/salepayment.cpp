#include "salepayment.h"

using namespace Utility;

SalePayment::SalePayment(double amount,
                         const Utility::PaymentMethod &method,
                         const Note &note) :
    amount(amount),
    method(method),
    note(note)
{ }

SalePayment::SalePayment(const QVariantMap &map) :
    amount(map.value("amount").toDouble()),
    method(map.value("payment_method").toString()),
    note(Note{ map.value("note").toString() })
{

}

QVariantMap SalePayment::toVariantMap() const
{
    return {
        { "amount", amount.toDouble() },
        { "payment_method", method.toString()},
        { "note", note.note }
    };
}
