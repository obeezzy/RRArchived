#include "saletransaction.h"

using namespace Utility;

SaleTransaction::SaleTransaction(const QVariantMap &map) :
    id(map.value("sale_transaction_id").toInt()),
    customer(Customer {
             map.value("customer_id", -1).toInt(),
             Client{ map.value("customer_name").toString() }
             }),
    totalCost(map.value("total_cost").toDouble()),
    amountPaid(map.value("amount_paid").toDouble()),
    balance(map.value("balance").toDouble()),
    discount(map.value("discount").toDouble()),
    payments(SalePaymentList{ map.value("payments").toList() }),
    products(SaleCartProductList{ map.value("products").toList() }),
    dueDateTime(map.value("due_date_time").toDateTime()),
    action(map.value("action").toString()),
    note(Note {
         map.value("note_id", -1).toInt(),
         map.value("note").toString()
         }),
    currency(map.value("currency").toString()),
    created(map.value("created").toDateTime()),
    lastEdited(map.value("last_edited").toDateTime()),
    user(User {
         map.value("user_id", -1).toInt(),
         map.value("user").toString()
         }),
    row(map.value("row", -1).toInt())
{
    flags.setFlag(RecordGroup::Suspended, map.value("suspended").toBool());
    flags.setFlag(RecordGroup::Archived, map.value("archived").toBool());
}

SaleTransaction::SaleTransaction(qint64 id) :
    id(id)
{}

SaleTransaction::SaleTransaction(qint64 id,
                                 const Customer &customer,
                                 qreal totalCost,
                                 qreal amountPaid,
                                 qreal balance,
                                 const RecordGroup::Flags &flags,
                                 const QDateTime &dueDateTime,
                                 const QString &action,
                                 const Note &note,
                                 const SalePaymentList &payments,
                                 const SaleCartProductList &products) :
    id(id),
    customer(customer),
    totalCost(totalCost),
    amountPaid(amountPaid),
    balance(balance),
    payments(payments),
    products(products),
    flags(flags),
    dueDateTime(dueDateTime),
    action(action),
    note(note)
{}

SaleTransaction::SaleTransaction(qint64 id,
                                 const Customer &customer,
                                 qreal totalCost,
                                 qreal amountPaid,
                                 qreal balance,
                                 const RecordGroup::Flags &flags,
                                 const Note &note) :
    id(id),
    customer(customer),
    totalCost(totalCost),
    amountPaid(amountPaid),
    balance(balance),
    flags(flags),
    note(note)
{}

QVariantMap SaleTransaction::toVariantMap() const
{
    return {
        { "sale_transaction_id", id },
        { "customer_id", customer.id },
        { "customer_name", customer.client.preferredName },
        { "total_cost", totalCost },
        { "amount_paid", amountPaid },
        { "balance", balance },
        { "discount", discount },
        { "payments", payments.toVariantList() },
        { "products", products.toVariantList() },
        { "suspended", flags.testFlag(RecordGroup::Suspended) },
        { "archived", flags.testFlag(RecordGroup::Archived) },
        { "due_date_time", dueDateTime },
        { "action", action },
        { "note_id", note.id },
        { "note", note.note },
        { "currency", currency },
        { "user_id", user.id },
        { "user", user.user },
        { "row", row }
    };
}
