#include "saletransaction.h"

using namespace Utility::Sales;

SaleTransaction::SaleTransaction(const QVariantMap &map) :
    id(map.value("sale_transaction_id").toInt()),
    customer(Utility::Customer {
             map.value("customer_id").toInt(),
             Utility::Client{ map.value("customer_name").toString() }
             }),
    monies(SaleMonies { QVariantMap {
            { "total_cost", map.value("total_cost").toDouble() },
            { "amount_paid", map.value("amount_paid").toDouble() },
            { "balance", map.value("balance").toDouble() },
            { "discount", map.value("discount").toDouble() }
           }}),
    products(SaleCartProductList{ map.value("products").toList() }),
    payments(SalePaymentList{ map.value("payments").toList() }),
    flags((map.value("suspended").toBool() ? Utility::RecordGroup::Suspended : Utility::RecordGroup::None)
          | (map.value("archived").toBool() ? Utility::RecordGroup::Archived : Utility::RecordGroup::None)),
    dueDateTime(map.value("due_date_time").toDateTime()),
    action(map.value("action").toString()),
    note(Note{ map }),
    timestamp(Utility::RecordTimestamp{ map }),
    user(User::User{ map }),
    row(map.value("row", -1).toInt())
{}

SaleTransaction::SaleTransaction(int id) :
    id(id)
{}

SaleTransaction::SaleTransaction(int id,
                                 const Utility::Customer &customer,
                                 const SaleMonies &monies,
                                 const SaleCartProductList &products,
                                 const SalePaymentList &payments,
                                 const Utility::RecordGroup::Flags &flags,
                                 const QDateTime &dueDateTime,
                                 const QString &action,
                                 const Utility::Note &note) :
    id(id),
    customer(customer),
    monies(monies),
    products(products),
    payments(payments),
    flags(flags),
    dueDateTime(dueDateTime),
    action(action),
    note(note)
{}

SaleTransaction::SaleTransaction(int id,
                                 const Utility::Customer &customer,
                                 const SaleMonies &monies,
                                 const Utility::RecordGroup::Flags &flags,
                                 const Utility::Note &note) :
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
        { "suspended", flags.testFlag(Utility::RecordGroup::Suspended) },
        { "archived", flags.testFlag(Utility::RecordGroup::Archived) },
        { "due_date_time", dueDateTime },
        { "action", action },
        { "note_id", note.id },
        { "note", note.note },
        { "user_id", user.id },
        { "user", user.user },
        { "row", row }
    };
}
