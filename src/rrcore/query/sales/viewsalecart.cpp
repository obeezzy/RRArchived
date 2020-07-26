#include "viewsalecart.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "database/databaseexception.h"
#include "user/userprofile.h"

using namespace Query::Sales;

ViewSaleCart::ViewSaleCart(int transactionId, QObject* receiver)
    : SaleExecutor(COMMAND,
                   {{"sale_transaction_id", transactionId},
                    {"sale_transaction_archived", false},
                    {"sold_product_archived", false}},
                   receiver)
{}

QueryResult ViewSaleCart::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    const QVariantMap& params{request().params()};

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        const auto& records(callProcedure(
            "ViewSaleCart",
            {ProcedureArgument{ProcedureArgument::Type::In,
                               "sale_transaction_id",
                               params.value("transaction_id")},
             ProcedureArgument{ProcedureArgument::Type::In,
                               "sale_transaction_archived",
                               params.value("sale_transaction_archived")},
             ProcedureArgument{ProcedureArgument::Type::In,
                               "sold_product_archived",
                               params.value("sold_product_archived")}}));

        QVariantList products;
        for (const auto& record : records)
            products.append(recordToMap(record));

        if (!products.isEmpty()) {
            QVariantMap outcome{products.first().toMap()};
            outcome.insert("products", products);
            outcome.insert("record_count", products.count());
            result.setOutcome(outcome);
        }

        return result;
    } catch (const DatabaseException&) {
        throw;
    }
}
