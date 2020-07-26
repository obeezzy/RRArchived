#ifndef DEBTTRANSACTION_H
#define DEBTTRANSACTION_H

#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <initializer_list>
#include "debtpayment.h"
#include "singletons/settings.h"
#include "utility/common/money.h"
#include "utility/common/note.h"
#include "utility/common/relatedtransaction.h"

namespace Utility {
namespace Debtor {
struct DebtTransaction
{
private:
    Q_GADGET
public:
    enum class State
    {
        Clean,
        Dirty,
        Fresh,
        Trash
    };
    Q_ENUM(State)
    int id{0};
    Money totalDebt;
    QDateTime dueDateTime;
    RelatedTransaction relatedTransaction;
    Note note;
    DebtPaymentList payments;
    DebtPaymentList archivedPayments;
    RecordTimestamp timestamp;
    State state{State::Clean};

    explicit DebtTransaction();
    explicit DebtTransaction(double totalDebt, const QDateTime& dueDateTime,
                             const Note& note = Note());
    explicit DebtTransaction(int id, const Money& totalDebt,
                             const QDateTime& dueDateTime,
                             const RelatedTransaction& relatedTransaction,
                             const Note& note, const QDateTime& created,
                             State state);
    explicit DebtTransaction(const QVariantMap& map);

    QVariantMap toVariantMap() const;

    inline void clear() { payments.clear(); }

    bool operator==(const DebtTransaction& other) { return id == other.id; }

    friend QDebug operator<<(QDebug debug,
                             const DebtTransaction& debtTransaction)
    {
        debug.nospace() << "DebtTransaction("
                        << "id=" << debtTransaction.id
                        << ", totalDebt=" << debtTransaction.totalDebt
                        << ", dueDate=" << debtTransaction.dueDateTime
                        << ", state=" << debtTransaction.state
                        << ", debtPayments=" << debtTransaction.payments << ")";

        return debug.nospace();
    }
};

class DebtTransactionList : public QList<DebtTransaction>
{
public:
    explicit DebtTransactionList() = default;
    DebtTransactionList(std::initializer_list<DebtTransaction> transactions)
        : QList<DebtTransaction>(transactions)
    {}
    explicit DebtTransactionList(const QVariantList& list)
        : QList<DebtTransaction>()
    {
        for (const auto& variant : list)
            append(DebtTransaction{variant.toMap()});
    }

    QVariantList toVariantList() const
    {
        QVariantList list;
        for (const auto& transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
}  // namespace Debtor
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::Debtor::DebtTransaction, Q_PRIMITIVE_TYPE);

#endif  // DEBTTRANSACTION_H
