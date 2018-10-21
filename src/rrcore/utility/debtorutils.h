#ifndef DEBTORUTILS_H
#define DEBTORUTILS_H

#include <QString>
#include <QDateTime>

struct DebtPayment {
    enum class State { Clean, Dirty, New };
    int id = -1;
    qreal amount = 0.0;
    QString note;
    State state;
}; Q_DECLARE_TYPEINFO(DebtPayment, Q_PRIMITIVE_TYPE);

struct DebtTransaction {
    enum class State { Clean, Dirty, New };
    int id = -1;
    qreal totalDebt = 0.0;
    QDateTime dueDateTime;
    QString note;
    QList<DebtPayment *> debtPayments;
    State state;
}; Q_DECLARE_TYPEINFO(DebtTransaction, Q_PRIMITIVE_TYPE);

#endif // DEBTORUTILS_H
