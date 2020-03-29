#ifndef DEBTPAYMENT_H
#define DEBTPAYMENT_H

#include "debtmonies.h"
#include "utility/common/note.h"
#include "utility/common/recordgroup.h"
#include "utility/common/recordtimestamp.h"
#include "singletons/settings.h"
#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QVariantList>

namespace Utility {
struct DebtPayment {
private:
    Q_GADGET
public:
    enum class State { Clean, Dirty, Fresh, Trash }; Q_ENUM(State)
    int id {0};
    int debtTransactionId {0};
    DebtMonies monies;
    QDateTime dueDateTime;
    RecordGroup::Flags flags;
    Note note;
    RecordTimestamp timestamp;
    State state {State::Fresh};
    static int temporaryId;

    explicit DebtPayment(const DebtMonies &monies,
                         const QDateTime &dueDateTime,
                         const Note &note = Note());
    explicit DebtPayment(int id,
                         const DebtMonies &monies,
                         const QDateTime &dueDateTime,
                         const RecordGroup::Flags &flags,
                         State state,
                         const Note &note = Note());

    explicit DebtPayment(const QVariantMap &map);
    QVariantMap toVariantMap() const;

    bool operator==(const DebtPayment &other) {
        return id == other.id;
    }

    friend QDebug operator<<(QDebug debug, const DebtPayment &debtPayment)
    {
        debug.nospace() << "DebtPayment("
                        << "id=" << debtPayment.id
                        << ", totalDebt=" << debtPayment.monies.totalDebt
                        << ", amountPaid=" << debtPayment.monies.amountPaid
                        << ", balance=" << debtPayment.monies.balance
                        << ", state=" << debtPayment.state
                        << ")";

        return debug.nospace();
    }
};

class DebtPaymentList : public QList<DebtPayment>
{
public:
    explicit DebtPaymentList() = default;
    explicit DebtPaymentList(std::initializer_list<DebtPayment> payments) :
        QList<DebtPayment>(payments) { }
    explicit DebtPaymentList(const QVariantList &list) :
        QList<DebtPayment>() {
        for (const auto &variant : list)
            append(DebtPayment{variant.toMap()});
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &payment : *this)
            list.append(payment.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::DebtPayment, Q_PRIMITIVE_TYPE);

#endif // DEBTPAYMENT_H
