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
    note(Note {
         map.value("note_id").toInt(),
         map.value("note").toString()
         })
{

}

QVariantMap PurchasePayment::toVariantMap() const
{
    return {
        { "amount", amount.toDouble() },
        { "payment_method", method.toString() },
        { "note", note.note },
        { "currency", amount.currency().isoCode() }
    };
}
