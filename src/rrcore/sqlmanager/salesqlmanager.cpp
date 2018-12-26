#include "salesqlmanager.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>

SaleSqlManager::SaleSqlManager(const QString &connectionName) :
    AbstractSqlManager(connectionName)
{

}

QueryResult SaleSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "add_sale_transaction")
            addSaleTransaction(request, result);
        else if (request.command() == "update_suspended_sale_transaction")
            updateSuspendedTransaction(request, result);
        else if (request.command() == "undo_add_sale_transaction")
            undoAddSaleTransaction(request, result);
        else if (request.command() == "view_sale_cart")
            viewSaleCart(request, result);
        else if (request.command() == "view_sale_transactions")
            viewSaleTransactions(request, result);
        else if (request.command() == "view_sale_transaction_items")
            viewSaleTransactionItems(request, result);
        else if (request.command() == "view_sale_home")
            viewSaleHome(request, result);
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

void SaleSqlManager::addSaleTransaction(const QueryRequest &request, QueryResult &result, bool skipSqlTransaction)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    const QVariantList &payments = params.value("payments").toList();
    const QVariantList &items = params.value("items").toList();
    int noteId = 0;
    int clientId = 0;
    int saleTransactionId = 0;
    int debtorId = 0;
    int debtTransactionId = 0;
    int creditorId = 0;
    int creditTransactionId = 0;

    QSqlQuery q(connection);

    try {
//        AbstractSqlManager::enforceArguments( { "action" }, params);

        if (!skipSqlTransaction && !DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(),
                                    QStringLiteral("Failed to start transation."));

        // STEP: Add client, if client does not exist.
        if (!params.value("customer_phone_number").toString().trimmed().isEmpty() && !params.value("suspended").toBool()) {
            const QList<QSqlRecord> records(callProcedure("AddClient", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "preferred_name",
                                                                  params.value("customer_name", QVariant::String)
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "phone_number",
                                                                  params.value("customer_phone_number", QVariant::String)
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  UserProfile::instance().userId()
                                                              }
                                                          }));

                clientId = records.first().value("id").toInt();
        }

        // STEP: Insert note, if available.
        if (!params.value("note").toString().trimmed().isEmpty()) {
            const QList<QSqlRecord> records(callProcedure("AddNote", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "note",
                                                                  params.value("note", QVariant::String)
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "table_name",
                                                                  QStringLiteral("sale_transaction")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "transaction_id",
                                                                  QVariant(QVariant::Int)
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  UserProfile::instance().userId()
                                                              }
                                                          }));

            noteId = records.first().value("id").toInt();
        }

        // STEP: Insert sale transaction.
        const QList<QSqlRecord> records(callProcedure("AddSaleTransaction", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "name",
                                                              params.value("customer_name", QVariant::String)
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "client_id",
                                                              clientId > 0 ? clientId : QVariant(QVariant::Int)
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "balance",
                                                              (params.value("action").toString() == "give_change" ?
                                                              0.0 : qAbs(params.value("balance").toDouble()))
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "discount",
                                                              params.value("discount", 0.0)
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "suspended",
                                                              params.value("suspended", false)
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "note",
                                                              params.value("note", QVariant::String)
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "user_id",
                                                              UserProfile::instance().userId()
                                                          }
                                                      }));

        saleTransactionId = records.first().value("id").toInt();

        // STEP: Insert sale payments.
        for (const QVariant &payment : payments) {
            const QVariantMap &paymentInfo = payment.toMap();
            const QList<QSqlRecord> records(callProcedure("AddSalePayment", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "sale_transaction_id",
                                                                  saleTransactionId
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "amount",
                                                                  paymentInfo.value("amount")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "method",
                                                                  params.value("method")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "currency",
                                                                  params.value("currency")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "note",
                                                                  params.value("note", QVariant::String)
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  UserProfile::instance().userId()
                                                              }
                                                          }));
        }

        for (const QVariant &item : items) {
            QVariantMap itemInfo = item.toMap();
            // STEP: Deduct quantity if:
            // 1. This is a non-suspended transaction.
            // 2. This is a suspended transaction and you want to reserve the goods for this customer.
            if (!params.value("suspended", false).toBool()) {
                const QList<QSqlRecord> records(callProcedure("DeductStockQuantity", {
                                                                  ProcedureArgument {
                                                                      ProcedureArgument::Type::In,
                                                                      "item_id",
                                                                      itemInfo.value("item_id")
                                                                  },
                                                                  ProcedureArgument {
                                                                      ProcedureArgument::Type::In,
                                                                      "quantity",
                                                                      itemInfo.value("quantity").toDouble()
                                                                  },
                                                                  ProcedureArgument {
                                                                      ProcedureArgument::Type::In,
                                                                      "unit_id",
                                                                      itemInfo.value("unit_id")
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
                                                                  },
                                                                  ProcedureArgument {
                                                                      ProcedureArgument::Type::Out,
                                                                      "initial_quantity_id",
                                                                      {}
                                                                  }
                                                              }));
            }

            const QList<QSqlRecord> records(callProcedure("AddSaleItem", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "sale_transaction_id",
                                                                  saleTransactionId
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "item_id",
                                                                  itemInfo.value("item_id")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "unit_id",
                                                                  itemInfo.value("unit_id")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "unit_price",
                                                                  itemInfo.value("unit_price")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "quantity",
                                                                  itemInfo.value("quantity")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "cost",
                                                                  itemInfo.value("cost")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "discount",
                                                                  itemInfo.value("discount")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "currency",
                                                                  QStringLiteral("NGN")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  UserProfile::instance().userId()
                                                              }
                                                          }));
        }

        // STEP: Insert debt or credit.
        if (!params.value("overlook_balance").toBool() && !params.value("suspended").toBool() && params.value("balance").toDouble() > 0.0) {
            QList<QSqlRecord> records(callProcedure("AddDebtor", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "client_id",
                                                                  clientId
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "note",
                                                                  {}
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  UserProfile::instance().userId()
                                                              }
                                                          }));
            debtorId = records.first().value("id").toInt();

            records = callProcedure("AddDebtTransaction", {
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "debtor_id",
                                            debtorId
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "transaction_table",
                                            "sale_transaction"
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "transaction_id",
                                            saleTransactionId
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "note",
                                            {}
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "user_id",
                                            UserProfile::instance().userId()
                                        }
                                    });

            debtTransactionId = records.first().value("id").toInt();

            records = callProcedure("AddDebtPayment", {
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "debt_transaction_id",
                                            debtTransactionId
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "total_amount",
                                            params.value("total_cost")
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "amount_paid",
                                            params.value("amount_paid")
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "balance",
                                            params.value("balance")
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "currency",
                                            QStringLiteral("NGN")
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "due_date",
                                            params.value("due_date")
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "note",
                                            {}
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "user_id",
                                            UserProfile::instance().userId()
                                        }
                                    });
        } else if (!params.value("overlook_balance").toBool() && !params.value("suspended").toBool() && params.value("balance").toDouble() < 0.0) {
            QList<QSqlRecord> records(callProcedure("AddCreditor", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "client_id",
                                                                  clientId
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "note",
                                                                  {}
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  UserProfile::instance().userId()
                                                              }
                                                          }));
            creditorId = records.first().value("id").toInt();

            records = callProcedure("AddCreditTransaction", {
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "creditor_id",
                                            debtorId
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "transaction_table",
                                            "sale_transaction"
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "transaction_id",
                                            saleTransactionId
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "note",
                                            {}
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "user_id",
                                            UserProfile::instance().userId()
                                        }
                                    });

            creditTransactionId = records.first().value("id").toInt();

            records = callProcedure("AddCreditPayment", {
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "credit_transaction_id",
                                            creditTransactionId
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "total_amount",
                                            params.value("total_cost")
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "amount_paid",
                                            params.value("amount_paid")
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "balance",
                                            params.value("balance")
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "currency",
                                            QStringLiteral("NGN")
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "due_date",
                                            params.value("due_date")
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "note",
                                            {}
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "user_id",
                                            UserProfile::instance().userId()
                                        }
                                    });
        }

        if (!skipSqlTransaction && !DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(),
                                    QStringLiteral("Failed to commit."));

        QVariantMap outcome;
        outcome.insert("client_id", clientId);
        outcome.insert("transaction_id", saleTransactionId);

        result.setOutcome(outcome);
    } catch (DatabaseException &) {
        if (!skipSqlTransaction && !DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void SaleSqlManager::updateSuspendedTransaction(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    const QDateTime &currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection);

    try {
        // STEP: Ensure suspended flag is set to true.
        if (!params.value("suspended").toBool())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateTransactionFailure, QString(), "Suspended flag must be set to true.");

        // STEP: Check if transaction is suspended. (VALIDATION)
        q.prepare("SELECT id FROM sale_transaction WHERE suspended = 1 AND archived = 0 AND id = :transaction_id");
        q.bindValue(":transaction_id", params.value("transaction_id"), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateTransactionFailure, q.lastError().text(), "Failed to fetch transaction ID.");

        if (!q.first())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateTransactionFailure, QString(), "Only suspended sale transactions can be updated.");

        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        addSaleTransaction(request, result, true);

        // STEP: Archive old sale transaction.
        q.prepare("UPDATE sale_transaction SET archived = 1, last_edited = :last_edited, user_id = :user_id WHERE id = :transaction_id");
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());
        q.bindValue(":transaction_id", params.value("transaction_id"));

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateTransactionFailure, q.lastError().text(), "Failed to archive sale transaction.");

        // STEP: Archive old sale item.
        q.prepare("UPDATE sale_item SET archived = 1, last_edited = :last_edited, user_id = :user_id "
                  "WHERE sale_transaction_id = :transaction_id");
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());
        q.bindValue(":transaction_id", params.value("transaction_id"));

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateTransactionFailure, q.lastError().text(), "Failed to archive sale item.");

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void SaleSqlManager::viewSaleCart(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QVariantMap params = request.params();
    QSqlQuery q(connection);
/*
SELECT sale_transaction.id as transaction_id,
sale_transaction.name as customer_name,
sale_transaction.client_id,
sale_transaction.total_cost,
sale_transaction.suspended,
sale_transaction.note_id,
sale_transaction.created,
sale_transaction.last_edited,
sale_transaction.user_id,
category.id as category_id,
category.category,
sale_item.item_id,
item.item,
sale_item.unit_price,
sale_item.quantity,
current_quantity.quantity as available_quantity,
unit.id as unit_id,
unit.unit,
sale_item.cost, sale_item.discount,
sale_item.currency, note.note FROM
(sale_item
INNER JOIN sale_transaction ON sale_item.sale_transaction_id = sale_transaction.id
INNER JOIN item ON sale_item.item_id = item.id
INNER JOIN unit ON sale_item.item_id = unit.item_id
INNER JOIN current_quantity ON sale_item.item_id = current_quantity.item_id)
INNER JOIN category ON item.category_id = category.id
LEFT JOIN note ON sale_item.note_id = note.id WHERE sale_transaction.archived = 0 AND sale_item.archived = 0 AND sale_transaction.id = 3;
*/
    try {
        q.prepare("SELECT sale_transaction.id as transaction_id, sale_transaction.name as customer_name, sale_transaction.client_id as client_id, "
                  "client.phone_number as customer_phone_number, "
                  "sale_transaction.total_cost, sale_transaction.suspended, sale_transaction.note_id, sale_transaction.created, "
                  "sale_transaction.last_edited, sale_transaction.user_id, category.id as category_id, category.category, "
                  "sale_item.item_id, item.item, sale_item.unit_price as unit_price, "
                  "sale_item.quantity, current_quantity.quantity as available_quantity, unit.id as unit_id, unit.unit, "
                  "unit.cost_price as cost_price, unit.retail_price as retail_price, "
                  "sale_item.cost, sale_item.discount, "
                  "sale_item.currency, note.note FROM (sale_item "
                  "INNER JOIN sale_transaction ON sale_item.sale_transaction_id = sale_transaction.id "
                  "INNER JOIN item ON sale_item.item_id = item.id "
                  "INNER JOIN unit ON sale_item.item_id = unit.item_id "
                  "INNER JOIN current_quantity ON sale_item.item_id = current_quantity.item_id) "
                  "INNER JOIN category ON item.category_id = category.id "
                  "LEFT JOIN client ON sale_transaction.client_id = client.id "
                  "LEFT JOIN note ON sale_item.note_id = note.id "
                  "WHERE sale_transaction.id = :transaction_id AND sale_transaction.archived = :sale_transaction_archived "
                  "AND sale_item.archived = :sale_item_archived");
        q.bindValue(":transaction_id", params.value("transaction_id"), QSql::Out);
        q.bindValue(":sale_transaction_archived", params.value("sale_transaction_archived", false), QSql::Out);
        q.bindValue(":sale_item_archived", params.value("sale_item_archived", false), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::ViewSaleTransactionFailure, q.lastError().text(), "Failed to fetch sale cart.");

        QVariantMap outcome;
        QVariantList items;
        while (q.next()) {
            items.append(recordToMap(q.record()));
        }

        if (!items.isEmpty()) {
            outcome.insert("transaction_id", params.value("transaction_id"));
            outcome.insert("client_id", items.first().toMap().value("client_id"));
            outcome.insert("customer_name", items.first().toMap().value("customer_name"));
            outcome.insert("customer_phone_number", items.first().toMap().value("customer_phone_number"));
            outcome.insert("total_cost", items.first().toMap().value("total_cost"));
            outcome.insert("items", items);
            outcome.insert("record_count", items.count());

            result.setOutcome(outcome);
        }
    } catch (DatabaseException &) {
        throw;
    }
}

void SaleSqlManager::undoAddSaleTransaction(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QVariantMap params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection);

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        if (params.value("client_id").toInt() > 0) {
            q.prepare("UPDATE client SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id WHERE id = :client_id");
            q.bindValue(":client_id", params.value("client_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UndoAddTransactionFailure, q.lastError().text(), "Failed to update client.");
        }

        if (params.value("transaction_id").toInt() > 0) {
            q.prepare("UPDATE sale_transaction SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id "
                      "WHERE id = :transaction_id");
            q.bindValue(":transaction_id", params.value("transaction_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UndoAddTransactionFailure, q.lastError().text(), "Failed to update sale transaction.");

            q.prepare("UPDATE debt_transaction SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id "
                      "WHERE :transaction_table = :transaction_table AND transaction_id = :transaction_id");
            q.bindValue(":transaction_table", "sale_transaction");
            q.bindValue(":transaction_id", params.value("transaction_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UndoAddTransactionFailure, q.lastError().text(), "Failed to update debt transaction.");

            q.prepare("UPDATE credit_transaction SET archived = 1 - archived, last_edited = :last_edited, user_id = :user_id "
                      "WHERE :transaction_table = :transaction_table AND transaction_id = :transaction_id");
            q.bindValue(":transaction_table", "sale_transaction");
            q.bindValue(":transaction_id", params.value("transaction_id"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UndoAddTransactionFailure, q.lastError().text(), "Failed to update credit transaction.");
        }


//        UPDATE current_quantity  INNER JOIN sale_item on current_quantity.item_id = sale_item.item_id
//                INNER JOIN sale_transaction ON sale_item.sale_transaction_id = sale_transaction.id SET
//                current_quantity.quantity = current_quantity.quantity + sale_item.quantity WHERE sale_transaction.id = 21;

        q.prepare("UPDATE current_quantity "
                  "INNER JOIN sale_item ON current_quantity.item_id = sale_item.item_id "
                  "INNER JOIN sale_transaction ON sale_item.sale_transaction_id = sale_transaction.id "
                  "SET current_quantity.quantity = current_quantity.quantity + sale_item.quantity, "
                  "current_quantity.last_edited = :last_edited, current_quantity.user_id = :user_id "
                  "WHERE sale_transaction.id = :transaction_id");
        q.bindValue(":transaction_id", params.value("transaction_id"));
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UndoAddTransactionFailure, q.lastError().text(), "Failed to update current quantity.");

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");

        result.setOutcome(request.params());
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void SaleSqlManager::viewSaleTransactions(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    QSqlQuery q(connection);

    try {
        const QList<QSqlRecord> records(callProcedure("ViewSaleTransactions", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "suspended",
                                                              params.value("suspended")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "archived",
                                                              params.value("archived")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "from",
                                                              params.value("from")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "to",
                                                              params.value("to")
                                                          }
                                                      }));

        QVariantList transactions;
        for (const QSqlRecord &record : records) {
            transactions.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap { { "transactions", transactions }, { "record_count", transactions.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}

void SaleSqlManager::viewSaleTransactionItems(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "transaction_id" }, params);

        q.prepare("SELECT category.id as category_id, category.category, sale_item.item_id, item.item, "
                  "sale_item.unit_price, sale_item.quantity, sale_item.unit_id, "
                  "unit.unit, sale_item.cost, sale_item.discount, sale_item.currency, sale_item.note_id, note.note, "
                  "sale_item.archived, sale_item.created, sale_item.last_edited, sale_item.user_id, user.user FROM sale_item "
                  "INNER JOIN item ON sale_item.item_id = item.id "
                  "INNER JOIN category ON category.id = item.category_id "
                  "INNER JOIN unit ON sale_item.unit_id = unit.id "
                  "INNER JOIN sale_transaction ON sale_transaction.id = sale_item.sale_transaction_id "
                  "LEFT JOIN user ON sale_item.user_id = user.id "
                  "LEFT JOIN note ON sale_transaction.note_id = note.id "
                  "WHERE sale_transaction_id = :transaction_id AND sale_transaction.suspended = :suspended "
                  "AND sale_transaction.archived = :archived");
        q.bindValue(":transaction_id", params.value("transaction_id"));
        q.bindValue(":suspended", params.value("suspended", false));
        q.bindValue(":archived", params.value("archived", false));

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::ViewSaleTransactionFailure,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to fetch sale items for transaction."));

        QVariantList items;
        while (q.next()) {
            items.append(recordToMap(q.record()));
        }

        result.setOutcome(QVariantMap { { "items", items }, { "record_count", items.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}

void SaleSqlManager::viewSaleHome(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QVariantMap params = request.params();
    QSqlQuery q(connection);
    QVariantList records;
    const int currentYear = QDate::currentDate().year();

    try {
        {
            /*
            SELECT DATE(created), SUM(amount_paid) FROM sale_transaction WHERE archived = 0 AND suspended = 0
            AND DATE(created) BETWEEN DATE('2018-03-05') AND CURDATE() GROUP BY DATE(created);
            */
            q.prepare("SELECT DATE(sale_transaction.created) AS created, SUM(sale_payment.amount) AS amount_paid FROM sale_transaction "
                      "INNER JOIN sale_payment ON sale_payment.sale_transaction_id = sale_transaction.id "
                      "WHERE sale_transaction.suspended = 0 AND sale_transaction.archived = 0 "
                      "AND DATE(sale_transaction.created) BETWEEN DATE(:from_date) AND DATE(:to_date) "
                      "GROUP BY DATE(sale_transaction.created)");
            q.bindValue(":from_date", params.value("from_date", QDate(QDate::currentDate().year(), 1, 1)));
            q.bindValue(":to_date", params.value("to_date", QDate::currentDate()));

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::ViewSaleHomeFailure, q.lastError().text(), "Failed to fetch total revenue.");

            QVariantList revenues;
            while (q.next()) {
                revenues.append(recordToMap(q.record()));
            }

            QVariantMap totalRevenueInfo;
            totalRevenueInfo.insert("data_type", "total_revenue");
            totalRevenueInfo.insert("data_model", revenues);

            if (!revenues.isEmpty())
                records.append(totalRevenueInfo);
        }
        {
            /*
SELECT category.id AS category_id,
category.category, sale_item.item_id,
item.item, SUM(sale_item.cost - sale_item.discount) AS total_cost,
SUM(sale_item.quantity) AS quantity
FROM sale_item
INNER JOIN item ON item.id = sale_item.item_id
INNER JOIN category ON category.id = item.category_id
WHERE sale_item.created BETWEEN '2018-01-01 00:00:00' AND '2019-01-01 00:00:00')
GROUP BY sale_item.item_id
ORDER BY SUM(sale_item.quantity) DESC
LIMIT 5;
            */
            q.prepare("SELECT category.id AS category_id, category.category, sale_item.item_id, item.item, "
                      "SUM(sale_item.cost - sale_item.discount) AS total_revenue, SUM(sale_item.quantity) AS total_quantity "
                      "FROM sale_item "
                      "INNER JOIN item ON item.id = sale_item.item_id "
                      "INNER JOIN category ON category.id = item.category_id "
                      "WHERE sale_item.created BETWEEN :from_date_time AND :to_date_time "
                      "GROUP BY sale_item.item_id "
                      "ORDER BY SUM(sale_item.quantity) DESC "
                      "LIMIT :limit");
            q.bindValue(":from_date_time", params.value("from_date_time", QDateTime(QDate(currentYear, 0, 0), QTime(0, 0))));
            q.bindValue(":to_date_time", params.value("to_date_time", QDateTime::currentDateTime()));
            q.bindValue(":limit", params.value("limit", 5));

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::ViewSaleHomeFailure, q.lastError().text(), "Failed to fetch most sold items.");

            QVariantList mostSoldItems;
            while (q.next()) {
                mostSoldItems.append(recordToMap(q.record()));
            }

            QVariantMap mostSoldItemsInfo;
            mostSoldItemsInfo.insert("data_type", "most_sold_items");
            mostSoldItemsInfo.insert("data_model", mostSoldItems);

            if (!mostSoldItems.isEmpty())
                records.append(mostSoldItemsInfo);
        }

        QVariantMap outcome;
        outcome.insert("record_count", records.count());
        outcome.insert("records", records);
        result.setOutcome(outcome);
    } catch (DatabaseException &) {
        throw;
    }
}
