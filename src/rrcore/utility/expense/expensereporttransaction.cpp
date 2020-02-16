#include "expensereporttransaction.h"

using namespace Utility;

ExpenseReportTransaction::ExpenseReportTransaction(const QVariantMap &map) :
    client(Client {
           map.value("client_name").toString()
           }),
    purpose(map.value("purpose").toString()),
    amount(map.value("amount").toDouble())
{}

QVariantMap ExpenseReportTransaction::toVariantMap() const
{
    return {
        { "client_name", client.preferredName },
        { "purpose", purpose },
        { "amount", amount }
    };
}
