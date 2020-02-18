#include "updatestockproduct.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"
#include "singletons/settings.h"
#include "utility/stockutils.h"
#include "database/exceptions/exceptions.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

Q_LOGGING_CATEGORY(lcupdatestockproduct, "rrcore.queryexecutors.stock.updatestockproduct")

using namespace StockQuery;

UpdateStockProduct::UpdateStockProduct(const Utility::StockProduct &product,
                                       QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "image_url", product.imageUrl },
                    { "category", product.category.category },
                    { "product", product.product },
                    { "description", product.description },
                    { "unit", product.unit.unit },
                    { "category_note", product.category.note.note },
                    { "product_note", product.note.note },
                    { "tracked", product.flags.testFlag(Utility::RecordGroup::Tracked) },
                    { "divisible", product.flags.testFlag(Utility::RecordGroup::Divisible) },
                    { "cost_price", product.costPrice },
                    { "retail_price", product.retailPrice },
                    { "base_unit_equivalent", product.unit.baseUnitEquivalent },
                    { "unit_preferred", product.unit.flags.testFlag(Utility::RecordGroup::Preferred) },
                    { "currency", product.currency },
                    { "product_id", product.id },
                    { "currency", product.currency }
                  }, receiver)
{

}

QueryResult UpdateStockProduct::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();
    int productCategoryId = 0;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    if (params.contains("quantity"))
        qCWarning(lcupdatestockproduct) << Q_FUNC_INFO
                                        << "-> This function is not responsible for updating quantity. Quantity will be ignored.";

    try {
        DatabaseUtils::beginTransaction(q);

        productCategoryId = addOrUpdateStockProductCategory();
        updateStockProduct(productCategoryId);
        updateStockProductUnit();

        DatabaseUtils::commitTransaction(q);
        return result;
    } catch (const DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

int UpdateStockProduct::addOrUpdateStockProductCategory()
{
    const QVariantMap &params = request().params();
    const QString &note = params.value("category_note").toString();
    const int noteId = QueryExecutor::addNote(note,
                                              QStringLiteral("product_category"),
                                              ExceptionPolicy::DisallowExceptions);

    const auto &records(callProcedure("AddOrUpdateStockProductCategory", {
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
                                              noteId
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "user_id",
                                              UserProfile::instance().userId()
                                          }
                                      }));

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral("Expected category ID, received nothing."));

    return records.first().value("product_category_id").toInt();
}

void UpdateStockProduct::updateStockProduct(int productCategoryId)
{
    const QVariantMap &params = request().params();
    const QByteArray &imageBlob = DatabaseUtils::imageUrlToByteArray(params.value("image_url").toUrl());
    const QString &note = params.value("product_note").toString();
    const int noteId = QueryExecutor::addNote(note,
                                              QStringLiteral("product"),
                                              ExceptionPolicy::DisallowExceptions);

    callProcedure("UpdateStockProduct", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "product_category_id",
                          productCategoryId
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
                          imageBlob
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "note_id",
                          noteId
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}

void UpdateStockProduct::updateStockProductUnit()
{
    const QVariantMap &params = request().params();
    const QString &note = params.value("product_unit_note").toString();
    const int noteId = QueryExecutor::addNote(note,
                                              QStringLiteral("product_unit"),
                                              ExceptionPolicy::DisallowExceptions);

    callProcedure("UpdateStockProductUnit", {
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
                          noteId
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}

