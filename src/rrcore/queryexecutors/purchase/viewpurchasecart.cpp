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
                        { "purchase_item_archived", false }
                     }, receiver)
{

}

QueryResult ViewPurchaseCart::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
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
                                                               "purchase_item_archived",
                                                               params.value("purchase_item_archived")
                                                           }
                                                       }));

        QVariantMap outcome;
        QVariantList items;
        for (const QSqlRecord &record : records) {
            items.append(recordToMap(record));
        }

        if (!items.isEmpty()) {
            outcome.insert("transaction_id", params.value("transaction_id"));
            outcome.insert("client_id", items.first().toMap().value("client_id"));
            outcome.insert("customer_name", items.first().toMap().value("customer_name"));
            outcome.insert("customer_phone_number", items.first().toMap().value("customer_phone_number"));
            outcome.insert("total_cost", items.first().toMap().value("total_cost"));
            outcome.insert("items", items);
            outcome.insert("record_count", items.count());

            result.setOutcome(outcome);
        }

        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
