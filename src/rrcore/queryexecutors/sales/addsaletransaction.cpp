#include "addsaletransaction.h"

using namespace SaleQuery;

AddSaleTransaction::AddSaleTransaction(qint64 transactionId,
                                       const QString &customerName,
                                       int clientId,
                                       const QString &customerPhoneNumber,
                                       qreal totalCost,
                                       qreal amountPaid,
                                       qreal balance,
                                       bool suspended,
                                       const QDateTime &dueDate,
                                       const QString &action,
                                       const QString &note,
                                       const SalePaymentList &payments,
                                       const StockItemList &items,
                                       QObject *receiver) :
    SaleExecutor(COMMAND, {
                    { "can_undo", true },
                    { "transaction_id", transactionId },
                    { "customer_name", customerName },
                    { "client_id", clientId },
                    { "customer_phone_number", customerPhoneNumber },
                    { "total_cost", totalCost },
                    { "amount_paid", amountPaid },
                    { "balance", balance },
                    { "suspended", suspended },
                    { "due_date", dueDate },
                    { "action", action },
                    { "note", note },
                    { "payments", payments.toVariantList() },
                    { "items", items.toVariantList() }
                 }, receiver)
{

}

QueryResult AddSaleTransaction::execute()
{
    QueryResult result{ request() };

    return result;
}
