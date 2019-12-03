#include "updatestockitem.h"

using namespace StockQuery;

UpdateStockItem::UpdateStockItem(int itemId,
                                 const QString &category,
                                 const QString &item,
                                 const QString &description,
                                 const QString &unit,
                                 bool tracked,
                                 bool divisible,
                                 qreal costPrice,
                                 qreal retailPrice,
                                 qreal baseUnitEquivalent,
                                 bool isPreferredUnit,
                                 const QString &currency,
                                 const QUrl &imageSource,
                                 const QString &categoryNote,
                                 const QString &itemNote,
                                 QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "image_source", imageSource },
                    { "category", category },
                    { "item", item },
                    { "description", description },
                    { "unit", unit },
                    { "category_note", categoryNote },
                    { "item_note", itemNote },
                    { "tracked", tracked },
                    { "divisible", divisible },
                    { "cost_price", costPrice },
                    { "retail_price", retailPrice },
                    { "base_unit_equivalent", baseUnitEquivalent },
                    { "is_preferred_unit", isPreferredUnit },
                    { "currency", currency },
                    { "item_id", itemId }
                  }, receiver)
{

}

QueryResult UpdateStockItem::execute()
{
    QueryResult result{ request() };

    return result;
}

