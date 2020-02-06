#include "updatestockproduct.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"
#include "singletons/settings.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

Q_LOGGING_CATEGORY(lcupdatestockproduct, "rrcore.queryexecutors.stock.updatestockproduct")

using namespace StockQuery;

UpdateStockProduct::UpdateStockProduct(int productId,
                                       const QString &category,
                                       const QString &product,
                                       const QString &description,
                                       const QString &unit,
                                       bool tracked,
                                       bool divisible,
                                       qreal costPrice,
                                       qreal retailPrice,
                                       qreal baseUnitEquivalent,
                                       bool isPreferredUnit,
                                       const QString &currency,
                                       const QUrl &imageUrl,
                                       const QString &categoryNote,
                                       const QString &productNote,
                                       QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "image_url", imageUrl },
                    { "category", category },
                    { "product", product },
                    { "description", description },
                    { "unit", unit },
                    { "category_note", categoryNote },
                    { "product_note", productNote },
                    { "tracked", tracked },
                    { "divisible", divisible },
                    { "cost_price", costPrice },
                    { "retail_price", retailPrice },
                    { "base_unit_equivalent", baseUnitEquivalent },
                    { "is_preferred_unit", isPreferredUnit },
                    { "currency", currency },
                    { "product_id", productId },
                    { "currency", currency },
                    { "preferred", true },
                    { "user_id", UserProfile::instance().userId() }
                  }, receiver)
{

}

QueryResult UpdateStockProduct::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    int categoryId = 0;

    QSqlQuery q(connection);

    if (params.contains("quantity"))
        qCWarning(lcupdatestockproduct) << Q_FUNC_INFO << "-> This function is not responsible for updating quantity. Quantity will be ignored.";

    try {
        DatabaseUtils::beginTransaction(q);

        // STEP: Insert category note.
        if (!params.value("category_note").toString().trimmed().isEmpty()) {
            QueryExecutor::addNote(params.value("category_note").toString(), "product");
        }

        // STEP: Update product note.
        if (!params.value("product_note").toString().trimmed().isEmpty()) {
            QueryExecutor::updateNote(params.value("product_note_id").toInt(), params.value("product_note").toString(), "product");
        }

        // STEP: Insert category if it doesn't exist. Else, get the category ID.
        QList<QSqlRecord> records(callProcedure("AddStockCategory", {
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "category",
                                                        params.value("category")
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "short_form",
                                                        {}
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "note_id",
                                                        {}
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "user_id",
                                                        params.value("user_id")
                                                    }
                                                }));
        categoryId = records.first().value("product_category_id").toInt();

        // STEP: Update product.
        callProcedure("UpdateStockProduct", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "product_category_id",
                              categoryId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "product_id",
                              params.value("product_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "product",
                              params.value("product")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "short_form",
                              params.value("short_form")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "description",
                              params.value("description")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "barcode",
                              params.value("barcode")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "divisible",
                              params.value("divisible")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "image",
                              DatabaseUtils::imageUrlToByteArray(params.value("image_url").toUrl()) // Store image as BLOB
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "note_id",
                              params.value("product_note_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              params.value("user_id")
                          }
                      });

        // STEP: Update unit.
        callProcedure("UpdateStockUnit", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "product_id",
                              params.value("product_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "unit",
                              params.value("unit")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "short_form",
                              params.value("short_form")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "base_unit_equivalent",
                              params.value("base_unit_equivalent")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "cost_price",
                              params.value("cost_price")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "retail_price",
                              params.value("retail_price")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "preferred",
                              params.value("preferred")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "currency",
                              params.value("currency")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "note_id",
                              QVariant(QVariant::Int)
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              params.value("user_id")
                          }
                      });

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

