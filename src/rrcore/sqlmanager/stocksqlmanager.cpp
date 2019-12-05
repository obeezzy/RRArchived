#include "stocksqlmanager.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlDriver>
#include <QImage>
#include <QByteArray>
#include <QBuffer>
#include <QSqlError>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

#include "user/userprofile.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"

StockSqlManager::StockSqlManager(const QString &connectionName)
    : AbstractSqlManager(connectionName)
{

}

QueryResult StockSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "add_new_stock_item")
            addNewStockItem(request);
        else if (request.command() == "move_item_to_stock_category")
            moveItemToStockCategory(request);
        else if (request.command() == "update_stock_item")
            updateStockItem(request);
        else if (request.command() == "view_stock_items")
            viewStockItems(request, result);
        else if (request.command() == "view_stock_item_details")
            viewStockItemDetails(request, result);
        else if (request.command() == "view_stock_categories")
            viewStockCategories(request, result);
        else if (request.command() == "remove_stock_item")
            removeStockItem(request);
        else if (request.command() == "undo_remove_stock_item")
            undoRemoveStockItem(request, result);
        else if (request.command() == "view_stock_report")
            viewStockReport(request, result);
        else if (request.command() == "view_stock_item_count")
            viewStockItemCount(request, result);
        else if (request.command() == "filter_stock_items")
            filterStockItems(request, result);
        else if (request.command() == "filter_stock_item_count")
            filterStockItemCount(request, result);
        else if (request.command() == "filter_stock_categories")
            filterStockCategories(request, result);
        else if (request.command() == "filter_stock_categories_by_item")
            filterStockCategoriesByItem(request, result);
        else
            throw DatabaseException(DatabaseError::QueryErrorCode::CommandNotFound,
                                    QString("Command not found: %1").arg(request.command()));

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

void StockSqlManager::addNewStockItem(const QueryRequest &request)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
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
                                                                  UserProfile::instance().userId()
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
                                                                  UserProfile::instance().userId()
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
                                                        UserProfile::instance().userId()
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
                                        DatabaseUtils::imageUrlToByteArray(params.value("image_url").toUrl()) // Store image as BLOB
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "note_id",
                                        itemNoteId > 0 ? itemNoteId : QVariant(QVariant::Int)
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "user_id",
                                        UserProfile::instance().userId()
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
                                        QStringLiteral("NGN")
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "note_id",
                                        QVariant(QVariant::Int)
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "user_id",
                                        UserProfile::instance().userId()
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
                              request.command()
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
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
                              UserProfile::instance().userId()
                          }
                      });

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void StockSqlManager::moveItemToStockCategory(const QueryRequest &request)
{
    const QVariantMap &params = request.params();

    try {
        callProcedure("MoveItemToStockCategory", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "category_id",
                              params.value("category_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "item_id",
                              params.value("item_id")
                          }
                      });

    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::updateStockItem(const QueryRequest &request)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    int categoryId = 0;

    QSqlQuery q(connection);

    if (params.contains("quantity"))
        qWarning() << Q_FUNC_INFO << "-> This function is not responsible for updating quantity. Quantity will be ignored.";

    try {
        DatabaseUtils::beginTransaction(q);

        // STEP: Insert category note.
        if (!params.value("category_note").toString().trimmed().isEmpty()) {
            AbstractSqlManager::addNote(params.value("category_note").toString(), "item");
        }

        // STEP: Update item note.
        if (!params.value("item_note").toString().trimmed().isEmpty()) {
            AbstractSqlManager::updateNote(params.value("item_note_id").toInt(), params.value("item_note").toString(), "item");
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
                                                        UserProfile::instance().userId()
                                                    }
                                                }));
        categoryId = records.first().value("id").toInt();

        // STEP: Update item.
        callProcedure("UpdateStockItem", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "category_id",
                              categoryId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "item_id",
                              params.value("item_id")
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
                              DatabaseUtils::imageUrlToByteArray(params.value("image_url").toUrl()) // Store image as BLOB
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "note_id",
                              params.value("item_note_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        // STEP: Update unit.
        callProcedure("UpdateStockUnit", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "item_id",
                              params.value("item_id")
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
                              1
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
                              params.value("preferred", true)
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "currency",
                              QStringLiteral("NGN")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "note_id",
                              QVariant(QVariant::Int)
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void StockSqlManager::viewStockItems(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewStockItems", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "category_id",
                                                               params.value("category_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_order",
                                                               params.value("sort_order").toInt() == Qt::DescendingOrder
                                                               ? "descending" : "ascending"
                                                           }
                                                       }));

        QVariantList items;
        for (const auto &record : records) {
            QVariantMap itemRecord{ recordToMap(record) };
            itemRecord.insert("image_url", DatabaseUtils::byteArrayToImageUrl(record.value("image").toByteArray()));
            itemRecord.remove("image");

            items.append(itemRecord);
        }

        result.setOutcome(QVariantMap {
                              { "items", items },
                              { "record_count", items.count() }
                          });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::viewStockItemCount(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();

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
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::viewStockItemDetails(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    try {
        enforceArguments({ "item_id" }, params);

        const QList<QSqlRecord> &records(callProcedure("ViewStockItemDetails", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "item",
                                                               params.value("item_id")
                                                           }
                                                       }));

        QVariantMap itemInfo;
        if (!records.isEmpty()) {
            itemInfo = recordToMap(records.first());
            itemInfo.insert("image_url", DatabaseUtils::byteArrayToImageUrl(itemInfo.value("image").toByteArray()));
            itemInfo.remove("image");
        }
        else
            throw DatabaseException(DatabaseError::QueryErrorCode::ViewStockItemDetailsFailed,
                                    QString(),
                                    QString("Item details do not exists for item '%1'.").arg(params.value("item_id").toString()));

        result.setOutcome(QVariantMap {
                              { "item", itemInfo },
                              { "record_count", 1 }
                          });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::viewStockCategories(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params(request.params());

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewStockCategories", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_order",
                                                               params.value("sort_order", "ascending")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "archived",
                                                               params.value("archived", false)
                                                           }
                                                       }));

        QVariantList categories;
        for (const auto &record : records) {
            categories.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap {
                              { "categories", categories },
                              { "record_count", categories.count() }
                          });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::removeStockItem(const QueryRequest &request)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "item_id" }, params);

        DatabaseUtils::beginTransaction(q);

        callProcedure("ArchiveStockItem", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "item_id",
                              params.value("item_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void StockSqlManager::undoRemoveStockItem(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection);

    try {
        enforceArguments({ "item_id" }, params);

        DatabaseUtils::beginTransaction(q);

        const QList<QSqlRecord> &records(callProcedure("UndoArchiveStockItem", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "item_id",
                                                               params.value("item_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_id",
                                                               UserProfile::instance().userId()
                                                           }
                                                       }));

        if (records.isEmpty())
            throw DatabaseException(DatabaseError::QueryErrorCode::EmptyResultSet,
                                    QString(),
                                    "No results returned.");

        QVariantMap outcome;
        result.setOutcome(QVariantMap {
                              { "category_id", records.first().value("category_id").toInt() },
                              { "category", records.first().value("category").toString() },
                              { "item_id", records.first().value("item_id").toInt() },
                              { "item_info", recordToMap(records.first()) },
                              { "item_row", params.value("item_row").toInt() }
                          });

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void StockSqlManager::viewStockReport(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewStockReport", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "from",
                                                               params.value("from")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "to",
                                                               params.value("to")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "filter_column",
                                                               params.value("filter_column")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "filter_text",
                                                               params.value("filter_text")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_column",
                                                               params.value("sort_column", QStringLiteral("category"))
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_order",
                                                               params.value("sort_order").toInt() == Qt::DescendingOrder
                                                               ? "descending" : "ascending"
                                                           }
                                                       }));

        QVariantList items;
        for (const QSqlRecord &record : records) {
            items.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap { { "items", items },
                                        { "record_count", items.count() },
                          });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::filterStockCategories(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params(request.params());

    try {
        const QList<QSqlRecord> &records(callProcedure("FilterStockCategories", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "filter_text",
                                                               params.value("filter_text")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_order",
                                                               params.value("sort_order")
                                                           }
                                                       }));

        QVector<int> categoryIds;
        QVariantList categories;
        for (const auto &record : records) {
            if (!categoryIds.contains(record.value("category_id").toInt())) {
                categories.append(recordToMap(record));
                categoryIds.append(record.value("category_id").toInt());
            }
        }

        result.setOutcome(QVariantMap {
                              { "categories", categories },
                              { "record_count", categories.count() }
                          });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::filterStockCategoriesByItem(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params(request.params());

    try {
        const QList<QSqlRecord> &records(callProcedure("FilterStockCategoriesByItem", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "filter_text",
                                                               params.value("filter_text")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_order",
                                                               params.value("sort_order")
                                                           }
                                                       }));

        QVector<int> categoryIds;
        QVariantList categories;
        for (const auto &record : records) {
            if (!categoryIds.contains(record.value("category_id").toInt())) {
                categories.append(recordToMap(record));
                categoryIds.append(record.value("category_id").toInt());
            }
        }

        result.setOutcome(QVariantMap {
                              { "categories", categories },
                              { "record_count", categories.count() }
                          });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::filterStockItems(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();

    try {
        const QList<QSqlRecord> &records(callProcedure("FilterStockItems", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "category_id",
                                                               params.value("category_id")
                                                           },
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
                                                               "sort_order",
                                                               params.value("sort_order")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "sort_column",
                                                               params.value("sort_column")
                                                           }
                                                       }));

        QVariantList items;
        for (const auto &record : records) {
            QVariantMap itemRecord { recordToMap(record) };
            itemRecord.insert("image_url", DatabaseUtils::byteArrayToImageUrl(record.value("image").toByteArray()));
            itemRecord.remove("image");

            items.append(itemRecord);
        }

        result.setOutcome(QVariantMap {
                              { "items", items },
                              { "record_count", items.count() }
                          });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::filterStockItemCount(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();

    try {
        const QList<QSqlRecord> &records(callProcedure("FilterStockItemCount", {
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

        int itemCount = 0;
        if (!records.isEmpty())
            itemCount = records.first().value("item_count").toInt();

        result.setOutcome(QVariantMap {
                              { "item_count", itemCount },
                              { "record_count", 1 }
                          });
    } catch (DatabaseException &) {
        throw;
    }
}
