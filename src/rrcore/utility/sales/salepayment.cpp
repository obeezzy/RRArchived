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

SalePayment::SalePayment(const QVariantMap &map) :
    amount(map.value("amount").toDouble()),
    method(map.value("payment_method").toString()),
    note(Note {
         map.value("note_id").toInt(),
         map.value("note").toString()
         })
{

}

QVariantMap SalePayment::toVariantMap() const
{
    return {
        { "amount", amount },
        { "payment_method", method.toString()},
        { "note", note.note }
    };
}
