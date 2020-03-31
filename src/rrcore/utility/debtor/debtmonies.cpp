#include "debtmonies.h"

using namespace Utility;

DebtMonies::DebtMonies(const QVariantMap &map) :
    debt(map.value("debt").toDouble()),
    amountPaid(map.value("amount_paid").toDouble()),
    balance(map.value("balance").toDouble())
{}

QVariantMap DebtMonies::toVariantMap() const
{
    return {
        { "debt", debt.toDouble() },
        { "amount_paid", amountPaid.toDouble() },
        { "balance", balance.toDouble() }
    };
}
