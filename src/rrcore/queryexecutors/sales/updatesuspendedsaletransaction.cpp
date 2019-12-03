#include "updatesuspendedsaletransaction.h"

using namespace SaleQuery;
UpdateSuspendedSaleTransaction::UpdateSuspendedSaleTransaction(qint64 transactionId,
                                                               const QString &customerName,
                                                               int clientId,
                                                               const QString &customerPhoneNumber,
                                                               qreal totalCost,
                                                               qreal amountPaid,
                                                               qreal balance,
                                                               const QString &note,
                                                               bool suspended,
                                                               QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "transaction_id", transactionId },
                    { "customer_name", customerName },
                    { "client_id", clientId },
                    { "customer_phone_number", customerPhoneNumber },
                    { "total_cost", totalCost },
                    { "amount_paid", amountPaid },
                    { "balance", balance },
                    { "note", note },
                    { "suspended", suspended }
                 }, receiver)
{

}

QueryResult UpdateSuspendedSaleTransaction::execute()
{
    QueryResult result{ request() };

    return result;
}
