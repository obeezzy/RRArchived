#include "addpurchasetransaction.h"

using namespace PurchaseQuery;

AddPurchaseTransaction::AddPurchaseTransaction(qint64 transactionId,
                                               int clientId,
                                               const QString &customerName, const QString &customerPhoneNumber,
                                               qreal totalCost,
                                               qreal amountPaid,
                                               qreal balance,
                                               bool suspended,
                                               const QDateTime &dueDate,
                                               const QString &action,
                                               const PurchasePaymentList &payments,
                                               const StockItemList &items,
                                               const QString &note,
                                               QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "can_undo", true },
                        { "transaction_id", transactionId },
                        { "client_id", clientId },
                        { "customer_name", customerName },
                        { "customer_phone_number", customerPhoneNumber },
                        { "total_cost", totalCost },
                        { "amount_paid", amountPaid },
                        { "balance", balance },
                        { "suspended", suspended },
                        { "due_date", dueDate },
                        { "action", action },
                        { "payments", payments.toVariantList() },
                        { "items", items.toVariantList() },
                        { "note", note }
                     }, receiver)
{

}

QueryResult AddPurchaseTransaction::execute()
{
    return PurchaseExecutor::addPurchaseTransaction(TransactionMode::UseSqlTransaction);
}
