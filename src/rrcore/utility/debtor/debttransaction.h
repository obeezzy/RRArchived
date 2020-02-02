#ifndef DEBTTRANSACTION_H
#define DEBTTRANSACTION_H

#include "utility/commonutils.h"
#include "singletons/settings.h"
#include "debtpayment.h"

#include <QString>
#include <QDateTime>
#include <QVariantList>
#include <QVariantMap>
#include <QDebug>

#include <initializer_list>

struct DebtTransaction {
private:
    Q_GADGET
public:
    enum class State { Clean, Dirty, Fresh, Trash }; Q_ENUM(State)
    int id;
    qreal totalDebt;
    QDateTime dueDateTime;
    RelatedTransaction relatedTransaction;
    Note note;
    DebtPaymentList debtPayments;
    DebtPaymentList archivedDebtPayments;
    QDateTime created;
    QDateTime lastEdited;
    State state;

    explicit DebtTransaction() :
        id(-1), totalDebt(0.0) { }
    explicit DebtTransaction(qreal totalDebt, const QDateTime &dueDateTime, const Note &note) :
        id(-1), totalDebt(totalDebt), dueDateTime(dueDateTime),
        note(note),
        state(State::Fresh) { }
    explicit DebtTransaction(int id, qreal totalDebt, const QDateTime &dueDateTime,
                    const RelatedTransaction &relatedTransaction,
                    const Note &note, const QDateTime &created, State state) :
        id(id), totalDebt(totalDebt), dueDateTime(dueDateTime),
        relatedTransaction(relatedTransaction), note(note),
        created(created), state(state) { }
    explicit DebtTransaction(const QVariantMap &map) :
        id(map.value("debt_transaction_id").toInt()),
        totalDebt(map.value("total_amount").toDouble()),
        dueDateTime(map.value("due_date").toDateTime()),
        note(map.value("note").toString()),
        state(State::Clean)
    { }

    void clear() { debtPayments.clear(); }

    bool operator==(const DebtTransaction &other) {
        return id == other.id;
    }

    QVariantMap toVariantMap() const {
        return {
            { "debt_transaction_id", id },
            { "total_amount", totalDebt },
            { "due_date", dueDateTime },
            { "related_transaction_id", relatedTransaction.id },
            { "related_transaction_table", relatedTransaction.tableName },
            { "note_id", note.id },
            { "note", note.note },
            { "last_edited", lastEdited }
        };
    }

    friend QDebug operator<<(QDebug debug, const DebtTransaction &debtTransaction)
    {
        debug.nospace() << "DebtTransaction("
                        << "id=" << debtTransaction.id
                        << ", totalDebt=" << debtTransaction.totalDebt
                        << ", dueDate=" << debtTransaction.dueDateTime
                        << ", state=" << debtTransaction.state
                        << ", debtPayments=" << debtTransaction.debtPayments
                        << ")";

        return debug.nospace();
    }
};
Q_DECLARE_TYPEINFO(DebtTransaction, Q_PRIMITIVE_TYPE);

class DebtTransactionList : public QList<DebtTransaction>
{
public:
    explicit DebtTransactionList() = default;
    DebtTransactionList(std::initializer_list<DebtTransaction> transactions) :
        QList<DebtTransaction>(transactions) { }
    explicit DebtTransactionList(const QVariantList &list) :
        QList<DebtTransaction>() {
        for (const auto &variant : list)
            append(DebtTransaction{variant.toMap()});
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};

#endif // DEBTTRANSACTION_H
