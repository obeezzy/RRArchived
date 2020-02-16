#include "purchasetransaction.h"

using namespace Utility;

PurchaseTransaction::PurchaseTransaction(const QVariantMap &transaction) :
    id(transaction.value("purchase_transaction_id").toLongLong()),
    vendor(Client {
           transaction.value("client_id").toInt(),
           transaction.value("client_preferred_name").toString()
           }),
    totalCost(transaction.value("total_cost").toDouble()),
    amountPaid(transaction.value("amount_paid").toDouble()),
    balance(transaction.value("balance").toDouble()),
    discount(transaction.value("discount").toDouble()),
    note(Note {
         transaction.value("note_id").toInt(),
         transaction.value("note").toString()
         }),
    created(transaction.value("created").toDateTime()),
    lastEdited(transaction.value("last_edited").toDateTime()),
    user(User{ transaction.value("user_id").toInt() })
{
    flags.setFlag(RecordGroup::Suspended, transaction.value("suspended").toBool());
    flags.setFlag(RecordGroup::Archived, transaction.value("archived").toBool());
}

PurchaseTransaction::PurchaseTransaction(qint64 id,
                                         const Vendor &vendor,
                                         qreal totalCost,
                                         qreal amountPaid,
                                         qreal balance,
                                         const RecordGroup::Flags &flags,
                                         const PurchaseCartProductList &products,
                                         const PurchasePaymentList &payments,
                                         const QDateTime &dueDateTime,
                                         const QString &action,
                                         const Note &note) :
    id(id),
    vendor(vendor),
    totalCost(totalCost),
    amountPaid(amountPaid),
    balance(balance),
    products(products),
    payments(payments),
    note(note),
    dueDateTime(dueDateTime),
    action(action),
    flags(flags)
{}

PurchaseTransaction::PurchaseTransaction(qint64 id,
                                         const Vendor &vendor,
                                         qreal totalCost,
                                         qreal amountPaid,
                                         qreal balance,
                                         const RecordGroup::Flags &flags,
                                         const PurchaseCartProductList &products,
                                         const PurchasePaymentList &payments,
                                         const Note &note) :
    id(id),
    vendor(vendor),
    totalCost(totalCost),
    amountPaid(amountPaid),
    balance(balance),
    products(products),
    payments(payments),
    note(note),
    flags(flags)
{}


QVariantMap PurchaseTransaction::toVariantMap() const
{
    return {
        { "purchase_transaction_id", id },
        { "client_id", vendor.client.id },
        { "client_preferred_name", vendor.client.preferredName },
        { "client_phone_number", vendor.client.phoneNumber },
        { "total_cost", totalCost },
        { "amount_paid", amountPaid },
        { "balance", balance },
        { "discount", discount },
        { "note_id", note.id },
        { "note", note.note },
        { "suspended", flags.testFlag(RecordGroup::Suspended) },
        { "archived", flags.testFlag(RecordGroup::Archived) },
        { "created", created },
        { "last_edited", lastEdited },
        { "user_id", user.id }
    };
}
