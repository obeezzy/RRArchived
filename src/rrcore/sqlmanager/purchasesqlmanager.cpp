#include "purchasesqlmanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include "database/databaseexception.h"

PurchaseSqlManager::PurchaseSqlManager(const QString &connectionName) :
    AbstractSqlManager (connectionName)
{

}

QueryResult PurchaseSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "view_purchase_home")
            viewPurchaseHome(request, result);
        else if (request.command() == "add_purchase_transaction")
            addPurchaseTransaction(request, result);
        else if (request.command() == "update_suspend_transaction")
            updateSuspendedTransaction(request, result);
        else if (request.command() == "view_purchase_cart")
            viewPurchaseCart(request, result);
        else if (request.command() == "undo_add_purchase_transaction")
            undoAddPurchaseTransaction(request, result);
        else if (request.command() == "view_purchase_transactions")
            viewPurchaseTransactions(request, result);
        else if (request.command() == "view_purchase_items")
            viewPurchaseTransactionItems(request, result);
        else
            throw DatabaseException(DatabaseException::RRErrorCode::CommandNotFound, QString("Command not found: %1").arg(request.command()));

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

void PurchaseSqlManager::viewPurchaseHome(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QVariantMap params = request.params();
    QSqlQuery q(connection);
    QVariantList homeRecords;
    const int currentYear = QDate::currentDate().year();

    try {
        /* Last Purchased Items */ {
            const QList<QSqlRecord> records(callProcedure("GetLastPurchasedItems", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "from_date_time",
                                                                  params.value("from_date_time", QDateTime(QDate(currentYear, 1, 1), QTime(0, 0)))
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "to_date_time",
                                                                  params.value("to_date_time", QDateTime::currentDateTime())
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "limit",
                                                                  params.value("limit", params.value("limit", 5))
                                                              }
                                                          }));

            QVariantList lastPurchasedItems;
            for (const QSqlRecord &record : records) {
                lastPurchasedItems.append(recordToMap(record));
            }

            QVariantMap lastPurchasedItemsInfo;
            lastPurchasedItemsInfo.insert("chart_type", "last_purchased_items");
            lastPurchasedItemsInfo.insert("chart_model", lastPurchasedItems);

            if (!lastPurchasedItems.isEmpty())
                homeRecords.append(lastPurchasedItems);
        }

        QVariantMap outcome;
        outcome.insert("record_count", homeRecords.count());
        outcome.insert("records", homeRecords);
        result.setOutcome(outcome);
    } catch (DatabaseException &) {
        throw;
    }
}

void PurchaseSqlManager::addPurchaseTransaction(const QueryRequest &request, QueryResult &result)
{
    Q_UNUSED(request)
    Q_UNUSED(result)
}

void PurchaseSqlManager::updateSuspendedTransaction(const QueryRequest &request, QueryResult &result)
{
    Q_UNUSED(request)
    Q_UNUSED(result)
}

void PurchaseSqlManager::viewPurchaseCart(const QueryRequest &request, QueryResult &result)
{
    Q_UNUSED(request)
    Q_UNUSED(result)
}

void PurchaseSqlManager::undoAddPurchaseTransaction(const QueryRequest &request, QueryResult &result)
{
    Q_UNUSED(request)
    Q_UNUSED(result)
}

void PurchaseSqlManager::viewPurchaseTransactions(const QueryRequest &request, QueryResult &result)
{
    Q_UNUSED(request)
    Q_UNUSED(result)
}

void PurchaseSqlManager::viewPurchaseTransactionItems(const QueryRequest &request, QueryResult &result)
{
    Q_UNUSED(request)
    Q_UNUSED(result)
}
