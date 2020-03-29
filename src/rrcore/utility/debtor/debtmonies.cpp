#include "debtmonies.h"

using namespace Utility;

DebtMonies::DebtMonies()
{}

DebtMonies::DebtMonies(const QVariantMap &map) :
    totalAmount(map.value("total_amount").toDouble()),
    amountPaid(map.value("amount_paid").toDouble()),
    balance(map.value("balance").toDouble())
{}

QVariantMap DebtMonies::toVariantMap() const
{
    return {
        { "total_amount", totalAmount.toDouble() },
        { "amount_paid", amountPaid.toDouble() },
        { "balance", amountPaid.toDouble() }
    };
}
