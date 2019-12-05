#include "viewstockitemcount.h"
#include "database/databaseexception.h"

using namespace StockQuery;

ViewStockItemCount::ViewStockItemCount(int categoryId,
                                       QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "category_id", categoryId <= 0 ? QVariant(QVariant::Int) : categoryId },
                    { "archived", false }
                  }, receiver)
{

}

QueryResult ViewStockItemCount::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params = request().params();

    try {
        const QList<QSqlRecord> records(callProcedure("ViewStockItemCount", {
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

        int itemCount = 0;
        if (!records.isEmpty())
            itemCount = records.first().value("item_count").toInt();

        result.setOutcome(QVariantMap {
                              { "item_count", itemCount },
                              { "record_count", 1 }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
