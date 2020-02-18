#include "filterstockproductcount.h"
#include "database/databaseexception.h"
#include "utility/commonutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace StockQuery;

FilterStockProductCount::FilterStockProductCount(const Utility::FilterCriteria &filterCriteria,
                                                 int categoryId,
                                                 QObject *receiver) :
    StockExecutor(COMMAND, {
                        { "filter_column", filterCriteria.column },
                        { "filter_text", filterCriteria.text },
                        { "category_id", categoryId }
                  }, receiver)
{

}

QueryResult FilterStockProductCount::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const auto &records(callProcedure("FilterStockProductCount", {
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "filter_text",
                                                  params.value("filter_text")
                                              },
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "filter_column",
                                                  params.value("filter_column")
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
