#include "viewpurchasecart.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace PurchaseQuery;

ViewPurchaseCart::ViewPurchaseCart(qint64 transactionId,
                                   QObject *receiver) :
    PurchaseExecutor(COMMAND, {
                        { "transaction_id", transactionId },
                        { "purchase_transaction_archived", false },
                        { "purchased_product_archived", false }
                     }, receiver)
{

}

QueryResult ViewPurchaseCart::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewPurchaseCart", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "transaction_id",
                                                               params.value("transaction_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "purchase_transaction_archived",
                                                               params.value("purchase_transaction_archived")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "purchased_product_archived",
                                                               params.value("purchased_product_archived")
                                                           }
                                                       }));

        QVariantMap outcome;
        QVariantList products;
        for (const auto &record : records)
            products.append(recordToMap(record));

        if (!products.isEmpty()) {
            outcome.insert("transaction_id", params.value("transaction_id"));
            outcome.insert("client_id", products.first().toMap().value("client_id"));
            outcome.insert("customer_name", products.first().toMap().value("customer_name"));
            outcome.insert("customer_phone_number", products.first().toMap().value("customer_phone_number"));
            outcome.insert("total_cost", products.first().toMap().value("total_cost"));
            outcome.insert("products", products);
            outcome.insert("record_count", products.count());

            result.setOutcome(outcome);
        }

        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
