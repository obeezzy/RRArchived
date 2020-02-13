#include "purchasepayment.h"

using namespace Utility;

PurchasePayment::PurchasePayment(qreal amount,
                                 const PaymentMethod &method,
                                 const Note &note,
                                 const QString &currency) :
    amount(amount),
    method(method),
    note(note),
    currency(currency)

{
}

QVariantMap PurchasePayment::toVariantMap() const
{
    return {
        { "amount", amount },
        { "payment_method", Utility::asString(method) },
        { "note", note.note }
    };
}
