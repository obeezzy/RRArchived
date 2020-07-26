#include "debttransaction.h"

using namespace Utility::Debtor;

DebtTransaction::DebtTransaction() {}

DebtTransaction::DebtTransaction(double totalDebt, const QDateTime& dueDateTime,
                                 const Note& note)
    : totalDebt(totalDebt),
      dueDateTime(dueDateTime),
      note(note),
      state(State::Fresh)
{}

DebtTransaction::DebtTransaction(int id, const Money& totalDebt,
                                 const QDateTime& dueDateTime,
                                 const RelatedTransaction& relatedTransaction,
                                 const Note& note, const QDateTime& created,
                                 State state)
    : id(id),
      totalDebt(totalDebt),
      dueDateTime(dueDateTime),
      relatedTransaction(relatedTransaction),
      note(note),
      timestamp(created, QDateTime()),
      state(state)
{}

DebtTransaction::DebtTransaction(const QVariantMap& map)
    : id(map.value("debt_transaction_id").toInt()),
      totalDebt(map.value("total_amount").toDouble()),
      dueDateTime(map.value("due_date").toDateTime()),
      note(map.value("note").toString()),
      state(State::Clean)
{}

QVariantMap DebtTransaction::toVariantMap() const
{
    return {{"debt_transaction_id", id},
            {"total_debt", totalDebt.toDouble()},
            {"due_date", dueDateTime},
            {"related_transaction_id", relatedTransaction.id},
            {"related_transaction_table", relatedTransaction.tableName},
            {"note_id", note.id},
            {"note", note.note},
            {"last_edited", timestamp.lastEdited}};
}
