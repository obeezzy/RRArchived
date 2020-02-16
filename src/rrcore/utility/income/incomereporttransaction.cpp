#include "incomereporttransaction.h"

using namespace Utility;

IncomeReportTransaction::IncomeReportTransaction(const QVariantMap &map) :
    client(Client {
           map.value("client_name").toString()
           }),
    purpose(map.value("purpose").toString()),
    amount(map.value("amount").toDouble())
{}

QVariantMap IncomeReportTransaction::toVariantMap() const
{
    return {
        { "client_name", client.preferredName },
        { "purpose", purpose },
        { "amount", amount }
    };
}
