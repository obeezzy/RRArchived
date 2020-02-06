#include "addstockproduct.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"
#include "utility/stockutils.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace StockQuery;

AddStockProduct::AddStockProduct(const StockProduct &product,
                           QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "image_url", product.imageUrl },
                    { "category_id", product.category.id },
                    { "product", product.product },
                    { "description", product.description },
                    { "unit", product.unit.unit },
                    { "category_note", product.category.note.note },
                    { "product_note", product.note.note },
                    { "tracked", product.tracked },
                    { "divisible", product.divisible },
                    { "cost_price", product.costPrice },
                    { "retail_price", product.retailPrice },
                    { "base_unit_equivalent", product.unit.baseUnitEquivalent },
                    { "is_preferred_unit", product.unit.preferred },
                    { "currency", product.currency },
                    { "quantity", product.quantity },
                    { "user_id", UserProfile::instance().userId() }
                  }, receiver)
{

}

QueryResult AddStockProduct::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    //const QByteArray &imageBytes = DatabaseUtils::imageUrlToByteArray(params.value("image_url").toUrl());
    int categoryNoteId = 0;
    int productNoteId = 0;
    int categoryId = 0;
    int productId = 0;
    int unitId = 0;

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        // STEP: Insert category note
        if (!params.value("category_note").toString().trimmed().isEmpty()) {
            const QList<QSqlRecord> records(callProcedure("AddNote", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "note",
                                                                  params.value("category_note", QVariant::String)
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "table_name",
                                                                  QStringLiteral("category")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  params.value("user_id")
                                                              }
                                                          }));

            categoryNoteId = records.first().value("id").toInt();
        }

        // STEP: Insert product note
        if (!params.value("product_note").toString().trimmed().isEmpty()) {
            const QList<QSqlRecord> records(callProcedure("AddNote", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "note",
                                                                  params.value("product_note", QVariant::String)
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "table_name",
                                                                  QStringLiteral("product")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  params.value("user_id")
                                                              }
                                                          }));

            productNoteId = records.first().value("note_id").toInt();
        }


        // STEP: Insert category
        QList<QSqlRecord> records(callProcedure("AddStockCategory", {
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "category",
                                                        params.value("category")
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "short_form",
                                                        params.value("short_form")
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "note_id",
                                                        categoryNoteId > 0 ? categoryNoteId : QVariant(QVariant::Int)
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "user_id",
                                                        params.value("user_id")
                                                    }
                                                }));

        categoryId = records.first().value("category_id").toInt();

        // STEP: Insert product.
        records = callProcedure("AddStockProduct", {
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "category_id",
                                        categoryId
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
                                        DatabaseUtils::imageUrlToByteArray(params.value("image_url").toUrl())// Store image as BLOB
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "note_id",
                                        productNoteId
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "user_id",
                                        params.value("user_id")
                                    }
                                });

        productId = records.first().value("product_id").toInt();

        // STEP: Insert unit.
        records = callProcedure("AddStockUnit", {
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "product_id",
                                        productId
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

        unitId = records.first().value("id").toInt();

        // STEP: Insert quantity into initial_quantity table.
        callProcedure("AddInitialQuantity", {
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
                              "product_unit_id",
                              unitId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "reason",
                              request().command()
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              params.value("user_id")
                          }
                      });

        // STEP: Insert unit into current_quantity table.
        callProcedure("AddCurrentQuantity", {
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
                              "product_unit_id",
                              unitId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              params.value("user_id")
                          }
                      });

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
    return result;
}
