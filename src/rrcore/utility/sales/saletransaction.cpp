#include "saletransaction.h"

using namespace Utility;

SaleTransaction::SaleTransaction(const QVariantMap &map)
{
    Q_UNUSED(map)
}

SaleTransaction::SaleTransaction(qint64 id) :
    id(id)
{}

SaleTransaction::SaleTransaction(qint64 id,
                                 const Customer &customer,
                                 qreal totalCost,
                                 qreal amountPaid,
                                 qreal balance,
                                 const RecordGroup::Flags &flags,
                                 const QDateTime &dueDateTime,
                                 const QString &action,
                                 const Note &note,
                                 const SalePaymentList &payments,
                                 const SaleCartProductList &products) :
    id(id),
    customer(customer),
    totalCost(totalCost),
    amountPaid(amountPaid),
    balance(balance),
    payments(payments),
    products(products),
    flags(flags),
    dueDateTime(dueDateTime),
    action(action),
    note(note)
{}

SaleTransaction::SaleTransaction(qint64 id,
                                 const Customer &customer,
                                 qreal totalCost,
                                 qreal amountPaid,
                                 qreal balance,
                                 const RecordGroup::Flags &flags,
                                 const Note &note) :
    id(id),
    customer(customer),
    totalCost(totalCost),
    amountPaid(amountPaid),
    balance(balance),
    flags(flags),
    note(note)
{}

QVariantMap SaleTransaction::toVariantMap() const
{
    return {};
}
