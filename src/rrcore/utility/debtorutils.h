#ifndef DEBTORUTILS_H
#define DEBTORUTILS_H

#include <QString>
#include <QDateTime>
#include <QVariantList>
#include <QVariantMap>

#include <initializer_list>

struct DebtPayment {
    enum class State { Clean, Dirty, New };
    int id;
    qreal amount;
    QString note;
    State state;

    bool operator==(const DebtPayment &other) {
        return id == other.id;
    }

    QVariantMap toVariantMap() const {
        return {
            { "debt_payment_id", id },
            { "amount", amount },
            { "note", note }
        };
    }
}; Q_DECLARE_TYPEINFO(DebtPayment, Q_PRIMITIVE_TYPE);

class DebtPaymentList : public QList<DebtPayment>
{
public:
    explicit DebtPaymentList() = default;
    DebtPaymentList(std::initializer_list<DebtPayment> payments) :
        QList<DebtPayment>(payments) { }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &payment : *this)
            list.append(payment.toVariantMap());
        return list;
    }
};

struct DebtTransaction {
    enum class State { Clean, Dirty, New };
    int id;
    qreal totalDebt;
    QDateTime dueDateTime;
    QString note;
    DebtPaymentList debtPayments;
    State state;

    bool operator==(const DebtTransaction &other) {
        return id == other.id;
    }

    QVariantMap toVariantMap() const {
        return {
            { "debt_transaction_id", id },
            { "total_debt", totalDebt },
            { "due_date", dueDateTime },
            { "note", note },
            { "debt_payments", debtPayments.toVariantList() }
        };
    }
}; Q_DECLARE_TYPEINFO(DebtTransaction, Q_PRIMITIVE_TYPE);

class DebtTransactionList : public QList<DebtTransaction>
{
public:
    explicit DebtTransactionList() = default;
    DebtTransactionList(std::initializer_list<DebtTransaction> transactions) :
        QList<DebtTransaction>(transactions) { }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};

#endif // DEBTORUTILS_H
