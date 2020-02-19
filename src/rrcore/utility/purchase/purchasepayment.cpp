#include "purchasepayment.h"

using namespace Utility;

PurchasePayment::PurchasePayment(qreal amount,
                                 const PaymentMethod &method,
                                 const Note &note) :
    amount(amount),
    method(method),
    note(note)
{
}

QVariantMap PurchasePayment::toVariantMap() const
{
    return {
        { "amount", amount },
        { "payment_method", method.toString() },
        { "note", note.note },
        { "currency", currency }
    };
}
