#ifndef SALETRANSACTION_H
#define SALETRANSACTION_H

#include "utility/commonutils.h"
#include "salecartproduct.h"
#include "salepayment.h"
#include <QString>

struct SaleTransaction
{
    int id;
    Customer customer;
    qreal totalCost;
    qreal amountPaid;
    qreal balance;
    SalePaymentList payments;
    SaleCartProductList products;
    RecordGroup::Flags flags;
    QDateTime dueDateTime;
    QString action;
    Note note;
    int row;

    explicit SaleTransaction(qint64 id);

    explicit SaleTransaction(qint64 id,
                             const Customer &customer,
                             qreal totalCost,
                             qreal amountPaid,
                             qreal balance,
                             const RecordGroup::Flags &flags,
                             const QDateTime &dueDateTime,
                             const QString &action,
                             const Note &note,
                             const SalePaymentList &payments,
                             const SaleCartProductList &products);

    explicit SaleTransaction(qint64 id,
                             const Customer &customer,
                             qreal totalCost,
                             qreal amountPaid,
                             qreal balance,
                             const RecordGroup::Flags &flags,
                             const Note &note);
};

#endif // SALETRANSACTION_H
