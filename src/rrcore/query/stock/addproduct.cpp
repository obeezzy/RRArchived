#include "addproduct.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"
#include "database/exceptions/exceptions.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace Query::Stock;

AddProduct::AddProduct(const Utility::Stock::Product &product,
                                 QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "product", product.product },
                    { "product_category_id", product.category.id },
                    { "product_category", product.category.category },
                    { "description", product.description },
                    { "image_url", product.imageUrl },
                    { "quantity", product.quantity.toDouble() },
                    { "product_unit", product.unit.unit },
                    { "category_note", product.category.note.note },
                    { "product_note", product.note.note },
                    { "tracked", product.flags.testFlag(Utility::RecordGroup::Tracked) },
                    { "divisible", product.flags.testFlag(Utility::RecordGroup::Divisible) },
                    { "cost_price", product.monies.costPrice.toDouble() },
                    { "retail_price", product.monies.retailPrice.toDouble() },
                    { "base_unit_equivalent", product.unit.baseUnitEquivalent },
                    { "unit_preferred", product.unit.flags.testFlag(Utility::RecordGroup::Preferred) },
                    { "currency", product.monies.costPrice.currency().isoCode() },
                  }, receiver)
{

}

QueryResult AddProduct::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    int productCategoryId = 0;
    int productId = 0;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        productCategoryId = addProductCategory();
        productId = addProduct(productCategoryId);
        addProductUnit(productId);
        addProductQuantity(productId);

        QueryExecutor::commitTransaction(q);
    } catch (const DatabaseException &) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
    return result;
}

int AddProduct::addProductCategory()
{
    const QVariantMap &params{ request().params() };
    const QString &note = params.value("category_note").toString();
    const int noteId = QueryExecutor::addNote(note,
                                              QStringLiteral("product_category"),
                                              ExceptionPolicy::DisallowExceptions);

    const auto &records(callProcedure("AddOrUpdateProductCategory", {
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "category",
                                              params.value("product_category")
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "user_id",
                                              UserProfile::instance().userId()
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "short_form",
                                              params.value("short_form")
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "note_id",
                                              noteId
                                          }
                                      }));

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral("Expected product category ID, received nothing."));

    return records.first().value("product_category_id").toInt();
}

int AddProduct::addProduct(int productCategoryId)
{
    const QVariantMap &params{ request().params() };
    const QByteArray &imageBlob = QueryExecutor::imageUrlToByteArray(params.value("image_url").toUrl());
    const QString &note = params.value("product_note").toString();
    const auto noteId = QueryExecutor::addNote(note,
                                              QStringLiteral("product"),
                                              ExceptionPolicy::DisallowExceptions);

    const auto &records = callProcedure("AddProduct", {
                                ProcedureArgument {
                                    ProcedureArgument::Type::In,
                                    "product_category_id",
                                    productCategoryId
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

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral("Expected product ID, received nothing."));

    return records.first().value("product_id").toInt();
}

int AddProduct::addProductUnit(int productId)
{
    const QVariantMap &params{ request().params() };
    const QString &note{ params.value("product_unit_note").toString() };
    const int noteId = QueryExecutor::addNote(note,
                                              QStringLiteral("unit"),
                                              ExceptionPolicy::DisallowExceptions);

    const auto &records = callProcedure("AddProductUnit", {
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "product_id",
                                                productId
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "unit",
                                                params.value("product_unit")
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
                                                params.value("unit_preferred")
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

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral("Expected product unit ID, received nothing."));

    return records.first().value("product_unit_id").toInt();
}

void AddProduct::addProductQuantity(int productId)
{
    const QVariantMap &params{ request().params() };
    const QString &reason{ request().command() };
    callProcedure("AddProductQuantity", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "product_id",
                          productId
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "quantity",
                          params.value("quantity")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "reason",
                          reason
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}
