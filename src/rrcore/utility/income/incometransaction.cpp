#include "incometransaction.h"
#include "utility/commonutils.h"

using namespace Utility;

IncomeTransaction::IncomeTransaction(const QVariantMap &map) :
    id(map.value("expense_transaction_id").toInt()),
    client(Client {
           map.value("client_name").toString()
           }),
    purpose(map.value("purpose").toString()),
    amount(map.value("amount").toDouble()),
    paymentMethod(Utility::PaymentMethod{ map.value("payment_method").toString() })
{

}

QVariantMap IncomeTransaction::toVariantMap() const
{
    return {
        { "expense_transaction_id", id },
        { "client_name", client.preferredName },
        { "purpose", purpose },
        { "amount", amount.toDouble() }
    };
}

