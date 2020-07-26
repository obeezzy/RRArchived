#include "viewpurchasedproducts.h"
#include "database/databaseexception.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace Query::Purchase;

ViewPurchasedProducts::ViewPurchasedProducts(int transactionId,
                                             QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "purchase_transaction_id", transactionId }
                     }, receiver)
{

}

QueryResult ViewPurchasedProducts::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "purchase_transaction_id" }, params);

        const QList<QSqlRecord> &records(callProcedure("ViewPurchasedProducts", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "purchase_transaction_id",
                                                               params.value("purchase_transaction_id")
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

        QVariantList products;
        for (const auto &record : records)
            products.append(recordToMap(record));

        result.setOutcome(QVariantMap {
                              { "products", products },
                              { "record_count", products.count() }
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
