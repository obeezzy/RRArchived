#include "saletransaction.h"

using namespace Utility;

SaleTransaction::SaleTransaction(const QVariantMap &map) :
    id(map.value("sale_transaction_id").toInt()),
    customer(Customer {
             map.value("customer_id", -1).toInt(),
             Client{ map.value("customer_name").toString() }
             }),
    monies(SaleMonies { QVariantMap {
            { "total_cost", map.value("total_cost").toDouble() },
            { "amount_paid", map.value("amount_paid").toDouble() },
            { "balance", map.value("balance").toDouble() },
            { "discount", map.value("discount").toDouble() }
           }}),
    payments(SalePaymentList{ map.value("payments").toList() }),
    products(SaleCartProductList{ map.value("products").toList() }),
    flags((map.value("suspended").toBool() ? RecordGroup::Suspended : RecordGroup::None)
          | (map.value("archived").toBool() ? RecordGroup::Archived : RecordGroup::None)),
    dueDateTime(map.value("due_date_time").toDateTime()),
    action(map.value("action").toString()),
    note(Note {
         map.value("note_id", -1).toInt(),
         map.value("note").toString()
         }),
    timestamp(RecordTimestamp {
              map.value("created").toDateTime(),
              map.value("last_edited").toDateTime()
              }),
    user(User {
         map.value("user_id", -1).toInt(),
         map.value("user").toString()
         }),
    row(map.value("row", -1).toInt())
{}

SaleTransaction::SaleTransaction(qint64 id) :
    id(id)
{}

SaleTransaction::SaleTransaction(qint64 id,
                                 const Customer &customer,
                                 const SaleMonies &monies,
                                 const RecordGroup::Flags &flags,
                                 const QDateTime &dueDateTime,
                                 const QString &action,
                                 const Note &note,
                                 const SalePaymentList &payments,
                                 const SaleCartProductList &products) :
    id(id),
    customer(customer),
    monies(monies),
    payments(payments),
    products(products),
    flags(flags),
    dueDateTime(dueDateTime),
    action(action),
    note(note)
{}

SaleTransaction::SaleTransaction(qint64 id,
                                 const Customer &customer,
                                 const SaleMonies &monies,
                                 const RecordGroup::Flags &flags,
                                 const Note &note) :
    id(id),
    customer(customer),
    monies(monies),
    flags(flags),
    note(note)
{}

QVariantMap SaleTransaction::toVariantMap() const
{
    return {
        { "sale_transaction_id", id },
        { "customer_id", customer.id },
        { "customer_name", customer.client.preferredName },
        { "total_cost", monies.totalCost.toDouble() },
        { "amount_paid", monies.amountPaid.toDouble() },
        { "balance", monies.balance.toDouble() },
        { "discount", monies.discount.toDouble() },
        { "payments", payments.toVariantList() },
        { "products", products.toVariantList() },
        { "suspended", flags.testFlag(RecordGroup::Suspended) },
        { "archived", flags.testFlag(RecordGroup::Archived) },
        { "due_date_time", dueDateTime },
        { "action", action },
        { "note_id", note.id },
        { "note", note.note },
        { "user_id", user.id },
        { "user", user.user },
        { "row", row }
    };
}
