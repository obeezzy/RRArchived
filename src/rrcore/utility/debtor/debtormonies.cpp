#include "debtormonies.h"

using namespace Utility::Debtor;

DebtorMonies::DebtorMonies(const QVariantMap& map)
    : debt(map.value("debt").toDouble()),
      amountPaid(map.value("amount_paid").toDouble()),
      balance(map.value("balance").toDouble())
{}

QVariantMap DebtorMonies::toVariantMap() const
{
    return {{"debt", debt.toDouble()},
            {"amount_paid", amountPaid.toDouble()},
            {"balance", balance.toDouble()}};
}
