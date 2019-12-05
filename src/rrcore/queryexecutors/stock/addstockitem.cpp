#include "addstockitem.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace StockQuery;

AddStockItem::AddStockItem(const QString &category,
                           const QString &item,
                           const QString &description,
                           double quantity,
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
                           const QString &itemNote,
                           QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "image", DatabaseUtils::imageUrlToByteArray(imageUrl) },
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
                    { "quantity", quantity },
                    { "user_id", UserProfile::instance().userId() }
                  }, receiver)
{

}

QueryResult AddStockItem::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    int categoryNoteId = 0;
    int itemNoteId = 0;
    int categoryId = 0;
    int itemId = 0;
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

        // STEP: Insert item note
        if (!params.value("item_note").toString().trimmed().isEmpty()) {
            const QList<QSqlRecord> records(callProcedure("AddNote", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "note",
                                                                  params.value("item_note", QVariant::String)
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "table_name",
                                                                  QStringLiteral("item")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  params.value("user_id")
                                                              }
                                                          }));

            itemNoteId = records.first().value("id").toInt();
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

        categoryId = records.first().value("id").toInt();

        // STEP: Insert item.
        records = callProcedure("AddStockItem", {
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "category_id",
                                        categoryId
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "item",
                                        params.value("item")
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
                                        itemNoteId > 0 ? itemNoteId : QVariant(QVariant::Int)
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "user_id",
                                        params.value("user_id")
                                    }
                                });

        itemId = records.first().value("id").toInt();

        // STEP: Insert unit.
        records = callProcedure("AddStockUnit", {
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "item_id",
                                        itemId
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
                                        "is_preferred_unit",
                                        params.value("is_preferred_unit")
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
                              "item_id",
                              itemId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "quantity",
                              params.value("quantity")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "unit_id",
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
                              "item_id",
                              itemId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "quantity",
                              params.value("quantity")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "unit_id",
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
