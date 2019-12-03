#include "updatesuspendedpurchasetransaction.h"

using namespace PurchaseQuery;

UpdateSuspendedPurchaseTransaction::UpdateSuspendedPurchaseTransaction(qint64 transactionId,
                                                                       int clientId,
                                                                       const QString &customerName,
                                                                       const QString &customerPhoneNumber,
                                                                       qreal totalCost,
                                                                       qreal amountPaid,
                                                                       qreal balance,
                                                                       bool suspended,
                                                                       const StockItemList &items,
                                                                       const QString &note,
                                                                       QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                            { "transaction_id", transactionId },
                            { "client_id", clientId },
                            { "customer_name", customerName },
                            { "customer_phone_number", customerPhoneNumber },
                            { "total_cost", totalCost },
                            { "amount_paid", amountPaid },
                            { "balance", balance },
                            { "suspended", suspended },
                            { "items", items.toVariantList() },
                            { "note", note }
                     }, receiver)
{

}

QueryResult UpdateSuspendedPurchaseTransaction::execute()
{
    QueryResult result{ request() };

    return result;
}
