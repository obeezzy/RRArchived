#include "debtpayment.h"

using namespace Utility;

int DebtPayment::temporaryId = 1;
const QString DEFAULT_CURRENCY = Utility::Currency::DEFAULT_CURRENCY;

DebtPayment::DebtPayment(const DebtMonies &monies,
                         const QDateTime &dueDateTime,
                         const Note &note) :
    id(temporaryId++),
    monies(monies),
    dueDateTime(dueDateTime),
    note(note)
{}

DebtPayment::DebtPayment(int id,
                         const DebtMonies &monies,
                         const QDateTime &dueDateTime,
                         const RecordGroup::Flags &flags,
                         State state,
                         const Note &note) :
    id(id),
    monies(monies),
    dueDateTime(dueDateTime),
    flags(flags),
    note(note),
    state(state)
{}

DebtPayment::DebtPayment(const QVariantMap &map) :
    id(map.value("debt_payment_id").toInt()),
    monies(DebtMonies { QVariantMap {
                        { "total_amount", map.value("total_amount").toDouble() },
                        { "amount_paid", map.value("amount_paid").toDouble() },
                        { "balance", map.value("balance").toDouble() },
                        { "currency", map.value("currency").toString() }
                 }}),
    dueDateTime(map.value("due_date").toDateTime()),
    note(Note {
         map.value("note_id").toInt(),
         map.value("note").toString()
         }),
    timestamp(RecordTimestamp {
              map.value("created").toDateTime(),
              map.value("last_edited").toDateTime()
              }),
    state(State::Clean)
{
    flags.setFlag(RecordGroup::Archived,
                  map.value("archived").toBool());
}

QVariantMap DebtPayment::toVariantMap() const {
    return {
        { "debt_transaction_id", debtTransactionId },
        { "debt_payment_id", id },
        { "total_amount", monies.totalAmount.toDouble() },
        { "amount_paid", monies.amountPaid.toDouble() },
        { "balance", monies.balance.toDouble() },
        { "currency", monies.amountPaid.currency().isoCode() },
        { "due_date", dueDateTime },
        { "note_id", note.id },
        { "note", note.note },
        { "archived", flags.testFlag(RecordGroup::Archived) },
        { "created", timestamp.created },
        { "last_edited", timestamp.lastEdited }
    };
}

