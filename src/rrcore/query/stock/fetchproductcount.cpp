#include "fetchproductcount.h"
#include "database/databaseexception.h"

using namespace Query::Stock;

FetchProductCount::FetchProductCount(int categoryId,
                                       QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "category_id", categoryId },
                    { "archived", false }
                  }, receiver)
{

}

QueryResult FetchProductCount::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> records(callProcedure("FetchProductCount", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "category_id",
                                                              params.value("product_category_id")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "archived",
                                                              params.value("archived")
                                                          }
                                                      }));

        int productCount = 0;
        if (!records.isEmpty())
            productCount = records.first().value("product_count").toInt();

        result.setOutcome(QVariantMap {
                              { "product_count", productCount },
                              { "record_count", 1 }
                          });
        return result;
    } catch (const DatabaseException &) {
        throw;
    }
}
