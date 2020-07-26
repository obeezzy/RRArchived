#include "purchasetransaction.h"

using namespace Utility::Purchase;

PurchaseTransaction::PurchaseTransaction(const QVariantMap& transaction)
    : id(transaction.value("purchase_transaction_id").toLongLong()),
      vendor(Client{transaction.value("client_id").toInt(),
                    transaction.value("client_preferred_name").toString()}),
      monies(PurchaseMonies{QVariantMap{
          {"total_cost", transaction.value("total_cost").toDouble()},
          {"amount_paid", transaction.value("amount_paid").toDouble()},
          {"balance", transaction.value("balance").toDouble()},
          {"discount", transaction.value("discount").toDouble()}}}),
      note(Note{transaction.value("note_id").toInt(),
                transaction.value("note").toString()}),
      timestamp(RecordTimestamp{transaction.value("created").toDateTime(),
                                transaction.value("last_edited").toDateTime()}),
      user(User::User{transaction.value("user_id").toInt()})
{
    flags.setFlag(Utility::RecordGroup::Suspended,
                  transaction.value("suspended").toBool());
    flags.setFlag(Utility::RecordGroup::Archived,
                  transaction.value("archived").toBool());
}

PurchaseTransaction::PurchaseTransaction(int id) : id(id) {}

PurchaseTransaction::PurchaseTransaction(
    int id, const Vendor& vendor, const PurchaseMonies& monies,
    const PurchaseCartProductList& products,
    const PurchasePaymentList& payments,
    const Utility::RecordGroup::Flags& flags, const QDateTime& dueDateTime,
    const QString& action, const Utility::Note& note)
    : id(id),
      vendor(vendor),
      monies(monies),
      products(products),
      payments(payments),
      dueDateTime(dueDateTime),
      action(action),
      flags(flags),
      note(note)
{}

PurchaseTransaction::PurchaseTransaction(
    int id, const Vendor& vendor, const PurchaseMonies& monies,
    const PurchaseCartProductList& products,
    const PurchasePaymentList& payments,
    const Utility::RecordGroup::Flags& flags, const Utility::Note& note)
    : id(id),
      vendor(vendor),
      monies(monies),
      products(products),
      payments(payments),
      flags(flags),
      note(note)
{}

QVariantMap PurchaseTransaction::toVariantMap() const
{
    return {{"purchase_transaction_id", id},
            {"client_id", vendor.client.id},
            {"client_preferred_name", vendor.client.preferredName},
            {"client_phone_number", vendor.client.phoneNumber},
            {"total_cost", monies.totalCost.toDouble()},
            {"amount_paid", monies.amountPaid.toDouble()},
            {"balance", monies.balance.toDouble()},
            {"discount", monies.discount.toDouble()},
            {"note_id", note.id},
            {"note", note.note},
            {"suspended", flags.testFlag(RecordGroup::Suspended)},
            {"archived", flags.testFlag(RecordGroup::Archived)},
            {"created", timestamp.created},
            {"last_edited", timestamp.lastEdited},
            {"user_id", user.id}};
}
