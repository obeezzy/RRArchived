#include "debtpayment.h"

using namespace Utility::Debtor;

int DebtPayment::temporaryId = 1;
const QString DEFAULT_CURRENCY = Utility::Currency::DEFAULT_CURRENCY;

DebtPayment::DebtPayment(const DebtorMonies& monies,
                         const QDateTime& dueDateTime, const Note& note)
    : id(temporaryId++),
      monies(monies),
      dueDateTime(dueDateTime),
      note(note)
{}

DebtPayment::DebtPayment(int id, const DebtorMonies& monies,
                         const QDateTime& dueDateTime,
                         const RecordGroup::Flags& flags, State state,
                         const Note& note)
    : id(id),
      monies(monies),
      dueDateTime(dueDateTime),
      flags(flags),
      note(note),
      state(state)
{}

DebtPayment::DebtPayment(const QVariantMap& map)
    : id(map.value("debt_payment_id").toInt()),
      monies(DebtorMonies{map}),
      dueDateTime(map.value("due_date").toDateTime()),
      flags(map.value("archived").toBool() ? RecordGroup::Archived
                                           : RecordGroup::None),
      note(Note{map}),
      timestamp(RecordTimestamp{map}),
      state(State::Clean)
{}

QVariantMap DebtPayment::toVariantMap() const
{
    return {{"debt_transaction_id", debtTransactionId},
            {"debt_payment_id", id},
            {"debt", monies.debt.toDouble()},
            {"amount_paid", monies.amountPaid.toDouble()},
            {"balance", monies.balance.toDouble()},
            {"currency", monies.amountPaid.currency().isoCode()},
            {"due_date", dueDateTime},
            {"note_id", note.id},
            {"note", note.note},
            {"archived", flags.testFlag(RecordGroup::Archived)},
            {"created", timestamp.created},
            {"last_edited", timestamp.lastEdited}};
}
