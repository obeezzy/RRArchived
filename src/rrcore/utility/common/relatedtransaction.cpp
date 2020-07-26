#include "relatedtransaction.h"
#include <QObject>

using namespace Utility;

RelatedTransaction::RelatedTransaction(int id, const QString& tableName)
    : id(id),
      tableName(tableName)
{}

QString RelatedTransaction::toString() const
{
    if (tableName == QStringLiteral("sale_transaction"))
        return QObject::tr("Sales");
    else if (tableName == QStringLiteral("purchase_transaction"))
        return QObject::tr("Purchase");
    else if (tableName == QStringLiteral("income_transaction"))
        return QObject::tr("Income");
    else if (tableName == QStringLiteral("expense_transaction"))
        return QObject::tr("Expense");
    else if (tableName == QStringLiteral("debtor"))
        return QObject::tr("Debtor");
    else if (tableName == QStringLiteral("creditor"))
        return QObject::tr("Creditor");

    return tableName;
}
