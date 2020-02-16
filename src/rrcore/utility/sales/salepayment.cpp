#include "salepayment.h"

using namespace Utility;

SalePayment::SalePayment(qreal amount,
                         const Utility::PaymentMethod &method,
                         const Note &note,
                         const QString &currency) :
    amount(amount),
    method(method),
    note(note),
    currency(currency)
{ }

QVariantMap SalePayment::toVariantMap() const
{
    return {
        { "amount", amount },
        { "payment_method", method.toString()},
        { "note", note.note }
    };
}
