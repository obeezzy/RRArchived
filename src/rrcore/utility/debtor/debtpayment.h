#ifndef DEBTPAYMENT_H
#define DEBTPAYMENT_H

#include "utility/commonutils.h"
#include "singletons/settings.h"

#include <QString>
#include <QDateTime>
#include <QVariantList>
#include <QVariantMap>
#include <QDebug>

#include <initializer_list>

struct DebtPayment {
private:
    Q_GADGET
public:
    enum class State { Clean, Dirty, Fresh, Trash }; Q_ENUM(State)
    int id;
    int debtTransactionId;
    qreal totalAmount;
    qreal amountPaid;
    qreal balance;
    QString currency;
    QDateTime dueDateTime;
    Note note;
    bool archived;
    QDateTime created;
    QDateTime lastEdited;
    State state;
    static int temporaryId;

    explicit DebtPayment(qreal totalAmount,
                         qreal amountPaid,
                         const QDateTime &dueDateTime,
                         const Note &note);

    explicit DebtPayment(int id,
                         qreal totalAmount,
                         qreal amountPaid,
                         qreal balance,
                         const QString &currency,
                         const QDateTime &dueDateTime,
                         const Note &note,
                         bool archived,
                         const QDateTime &created,
                         const QDateTime &lastEdited,
                         State state);

    explicit DebtPayment(const QVariantMap &map);

    QVariantMap toVariantMap() const;

    bool operator==(const DebtPayment &other) {
        return id == other.id;
    }

    friend QDebug operator<<(QDebug debug, const DebtPayment &debtPayment)
    {
        debug.nospace() << "DebtPayment("
                        << "id=" << debtPayment.id
                        << ", totalAmount=" << debtPayment.totalAmount
                        << ", amountPaid=" << debtPayment.amountPaid
                        << ", balance=" << debtPayment.balance
                        << ", state=" << debtPayment.state
                        << ")";

        return debug.nospace();
    }
}; Q_DECLARE_TYPEINFO(DebtPayment, Q_PRIMITIVE_TYPE);

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

#endif // DEBTPAYMENT_H
