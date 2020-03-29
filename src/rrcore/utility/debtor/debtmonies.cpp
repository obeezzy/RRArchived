#include "debtmonies.h"

using namespace Utility;

DebtMonies::DebtMonies()
{}

DebtMonies::DebtMonies(const QVariantMap &map) :
    totalDebt(map.value("total_debt").toDouble()),
    amountPaid(map.value("amount_paid").toDouble()),
    balance(map.value("balance").toDouble())
{}

QVariantMap DebtMonies::toVariantMap() const
{
    return {
        { "total_debt", totalDebt.toDouble() },
        { "amount_paid", amountPaid.toDouble() },
        { "balance", amountPaid.toDouble() }
    };
}
