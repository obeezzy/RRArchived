#include "expensetransaction.h"

using namespace Utility;

ExpenseTransaction::ExpenseTransaction(const QVariantMap &map) :
    id(map.value("expense_transaction_id").toInt()),
    client(Client { map }),
    purpose(map.value("purpose").toString()),
    amount(map.value("amount").toDouble()),
    paymentMethod(map.value("payment_method").toString())
{}

QVariantMap ExpenseTransaction::toVariantMap() const
{
    return {
        { "expense_transaction_id", id },
        { "client_id", client.id },
        { "preferred_name", client.preferredName },
        { "purpose", purpose },
        { "amount", amount.toDouble() },
        { "payment_method", paymentMethod.toString() }
    };
}
