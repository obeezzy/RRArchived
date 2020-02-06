#include "fetchstockproductcount.h"
#include "database/databaseexception.h"

using namespace StockQuery;

FetchStockProductCount::FetchStockProductCount(int categoryId,
                                               QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "category_id", categoryId <= 0 ? QVariant(QVariant::Int) : categoryId },
                    { "archived", false }
                  }, receiver)
{

}

QueryResult FetchStockProductCount::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> records(callProcedure("FetchStockProductCount", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "category_id",
                                                              params.value("category_id")
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
    } catch (DatabaseException &) {
        throw;
    }
}
