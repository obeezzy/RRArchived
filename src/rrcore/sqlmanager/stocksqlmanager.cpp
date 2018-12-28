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

#include "singletons/userprofile.h"
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
        else if (request.command() == "update_stock_item")
            updateStockItem(request);
        else if (request.command() == "view_stock_items")
            viewStockItems(request, result);
        else if (request.command() == "view_stock_item_details")
            viewStockItemDetails(request, result);
        else if (request.command() == "view_stock_categories")
            viewStockCategories(request, result);
        else if (request.command() == "remove_stock_item")
            removeStockItem(request, result);
        else if (request.command() == "undo_remove_stock_item")
            undoRemoveStockItem(request, result);
        else
            throw DatabaseException(DatabaseException::RRErrorCode::CommandNotFound, QString("Command not found: %1").arg(request.command()));

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
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

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
                                        DatabaseUtils::imageToByteArray(params.value("image_source").toString())// Store image as BLOB
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
                                        true
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

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

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
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

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
                              DatabaseUtils::imageToByteArray(params.value("image_source").toString()) // Store image as BLOB
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
                              params.value("preferred")
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

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void StockSqlManager::viewStockItems(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();

    try {
        const QList<QSqlRecord> records(callProcedure("ViewStockItems", {
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


        QStringList categories;
        QVariantList itemGroups;
        int itemCount = 0;
        for (int i = 0; i < records.count(); ++i) {
            auto record = records[i];
            const auto categoryId = record.value("category_id").toInt();
            const auto &category = record.value("category").toString();

            QVariantList items;

            while ((i < records.count()) && categoryId == record.value("category_id").toInt()) {
                QVariantMap itemRecord;
                itemRecord.insert("item_id", record.value("item_id"));
                itemRecord.insert("category_id", categoryId);
                itemRecord.insert("category", category);
                itemRecord.insert("item", record.value("item"));
                itemRecord.insert("description", record.value("description"));
                itemRecord.insert("divisible", record.value("divisible"));
                itemRecord.insert("image", DatabaseUtils::byteArrayToImage(record.value("image").toByteArray()));
                itemRecord.insert("quantity", record.value("quantity"));
                itemRecord.insert("unit", record.value("unit"));
                itemRecord.insert("unit_id", record.value("unit_id"));
                itemRecord.insert("cost_price", record.value("cost_price"));
                itemRecord.insert("retail_price", record.value("retail_price"));
                itemRecord.insert("currency", record.value("currency"));
                itemRecord.insert("created", record.value("created"));
                itemRecord.insert("last_edited", record.value("last_edited"));
                itemRecord.insert("user", record.value("user"));

                items.append(itemRecord);
                itemCount++;

                if ((i + 1) < records.count() && categoryId == records.at(i + 1).value("category_id").toInt())
                    record = records[++i];
                else
                    break;
            }

            categories.append(category);
            itemGroups.append(QVariant(items));
        }

        if (categories.count() != itemGroups.count())
            throw DatabaseException(DatabaseException::RRErrorCode::ResultMismatch,
                                    QString("Category count (%1) and item group count (%2) are unequal.")
                                    .arg(categories.count()).arg(itemGroups.count()));

        result.setOutcome(QVariantMap { { "categories", categories },
                                        { "item_groups", itemGroups },
                                        { "record_count", itemCount },
                                        { "total_items", itemCount }
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

        const QList<QSqlRecord> records(callProcedure("ViewStockItemDetails", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::InOut,
                                                              "item",
                                                              params.value("item_id")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "category_id",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "category",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "item",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "description",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "divisible",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "image",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "quantity",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "unit_id",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "unit",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "cost_price",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "retail_price",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "currency",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "created",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "last_edited",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "user_id",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "user",
                                                              {}
                                                          }
                                                      }));

        QVariantMap itemInfo;
        if (!records.isEmpty())
            itemInfo = recordToMap(records.first());
        else
            throw DatabaseException(DatabaseException::RRErrorCode::ViewStockItemDetailsFailed,
                                    QString(),
                                    QString("Item details do not exists for item '%1'.").arg(params.value("item_id").toString()));

        result.setOutcome(QVariantMap { { "item", itemInfo }, { "record_count", 1 } });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::viewStockCategories(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params(request.params());

    try {
        QList<QSqlRecord> records(callProcedure("ViewStockCategories", {
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "sort_order",
                                                        params.value("sort_order")
                                                    }
                                                }));

        QVariantList categories;
        for (const auto &record : records) {
            categories.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap { { "categories", categories }, { "record_count", categories.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::removeStockItem(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection);

    try {
        enforceArguments({ "item_id" }, params);

        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        const QList<QSqlRecord> records(callProcedure("ArchiveStockItem", {
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
            throw DatabaseException(DatabaseException::RRErrorCode::EmptyResultSet, QString(), "No results returned.");

        QVariantMap outcome;
        outcome.insert("category_id", records.first().value("category_id"));
        outcome.insert("item_id", records.first().value("item_id"));

        result.setOutcome(outcome);

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

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

        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        const QList<QSqlRecord> records(callProcedure("UndoArchiveStockItem", {
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
            throw DatabaseException(DatabaseException::RRErrorCode::EmptyResultSet, QString(), "No results returned.");

        QVariantMap outcome;
        outcome.insert("category_id", records.first().value("category_id"));
        outcome.insert("item_id", records.first().value("item_id"));
        outcome.insert("item_info", recordToMap(records.first()));

        result.setOutcome(outcome);

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}
