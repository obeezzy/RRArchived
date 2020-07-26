#include "filtercategoriesbyproduct.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "database/databaseexception.h"
#include "utility/common/filtercriteria.h"
#include "utility/common/sortcriteria.h"

using namespace Query::Stock;

FilterCategoriesByProduct::FilterCategoriesByProduct(
    const Utility::FilterCriteria& filterCriteria,
    const Utility::SortCriteria& sortCriteria,
    const Utility::RecordGroup::Flags& flags, QObject* receiver)
    : StockExecutor(
          COMMAND,
          {{"filter_text", filterCriteria.text},
           {"sort_order", sortCriteria.orderAsString()},
           {"archived", flags.testFlag(Utility::RecordGroup::Archived)}},
          receiver)
{}

QueryResult FilterCategoriesByProduct::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    const QVariantMap& params(request().params());

    try {
        const auto& records(callProcedure(
            "FilterCategoriesByProduct",
            {ProcedureArgument{ProcedureArgument::Type::In, "filter_text",
                               params.value("filter_text")},
             ProcedureArgument{ProcedureArgument::Type::In, "sort_order",
                               params.value("sort_order")}}));

        QVector<int> categoryIds;
        QVariantList categories;
        for (const auto& record : records) {
            if (!categoryIds.contains(
                    record.value("product_category_id").toInt())) {
                categories.append(recordToMap(record));
                categoryIds.append(record.value("product_category_id").toInt());
            }
        }

        result.setOutcome(QVariantMap{{"categories", categories},
                                      {"record_count", categories.count()}});
        return result;
    } catch (const DatabaseException&) {
        throw;
    }
}
