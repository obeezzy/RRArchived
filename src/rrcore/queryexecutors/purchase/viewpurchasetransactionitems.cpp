#include "viewpurchasetransactionitems.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace PurchaseQuery;

ViewPurchaseTransactionItems::ViewPurchaseTransactionItems(qint64 transactionId,
                                                           QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "transaction_id", transactionId }
                     }, receiver)
{

}

QueryResult ViewPurchaseTransactionItems::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "transaction_id" }, params);

        const QList<QSqlRecord> &records(callProcedure("ViewPurchaseTransactionItems", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "transaction_id",
                                                               params.value("transaction_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "suspended",
                                                               params.value("suspended", false)
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "archived",
                                                               params.value("archived", false)
                                                           }
                                                       }));

        QVariantList items;
        for (const QSqlRecord &record : records) {
            items.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap { { "items", items }, { "record_count", items.count() } });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
