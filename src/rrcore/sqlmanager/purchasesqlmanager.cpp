#include "purchasesqlmanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

PurchaseSqlManager::PurchaseSqlManager(const QString &connectionName) :
    AbstractSqlManager (connectionName)
{

}

QueryResult PurchaseSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "view_purchase_home")
            viewPurchaseHome(request, result);
        else if (request.command() == "add_purchase_transaction")
            addPurchaseTransaction(request, result);
        else if (request.command() == "update_suspended_transaction")
            updateSuspendedTransaction(request, result);
        else if (request.command() == "view_purchase_cart")
            viewPurchaseCart(request, result);
        else if (request.command() == "undo_add_purchase_transaction")
            undoAddPurchaseTransaction(request, result);
        else if (request.command() == "view_purchase_transactions")
            viewPurchaseTransactions(request, result);
        else if (request.command() == "view_purchase_transaction_items")
            viewPurchaseTransactionItems(request, result);
        else if (request.command() == "remove_purchase_transaction")
            removePurchaseTransaction(request, result);
        else if (request.command() == "undo_remove_purchase_transaction")
            undoRemovePurchaseTransaction(request, result);
        else if (request.command() == "view_purchase_report")
            viewPurchaseReport(request, result);
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

void PurchaseSqlManager::viewPurchaseHome(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QVariantMap params = request.params();
    QSqlQuery q(connection);
    QVariantList homeRecords;
    const int currentYear = QDate::currentDate().year();

    try {
        /* Last Purchased Items */ {
            const QList<QSqlRecord> &records(callProcedure("GetLastPurchasedItems", {
                                                               ProcedureArgument {
                                                                   ProcedureArgument::Type::In,
                                                                   "from_date_time",
                                                                   params.value("from_date_time", QDateTime(QDate(currentYear, 1, 1), QTime(0, 0)))
                                                               },
                                                               ProcedureArgument {
                                                                   ProcedureArgument::Type::In,
                                                                   "to_date_time",
                                                                   params.value("to_date_time", QDateTime::currentDateTime())
                                                               },
                                                               ProcedureArgument {
                                                                   ProcedureArgument::Type::In,
                                                                   "limit",
                                                                   params.value("limit", params.value("limit", 5))
                                                               }
                                                           }));

            QVariantList lastPurchasedItems;
            for (const QSqlRecord &record : records) {
                lastPurchasedItems.append(recordToMap(record));
            }

            QVariantMap lastPurchasedItemsInfo;
            lastPurchasedItemsInfo.insert("chart_type", "last_purchased_items");
            lastPurchasedItemsInfo.insert("chart_model", lastPurchasedItems);

            if (!lastPurchasedItems.isEmpty())
                homeRecords.append(lastPurchasedItems);
        }

        QVariantMap outcome;
        outcome.insert("record_count", homeRecords.count());
        outcome.insert("records", homeRecords);
        result.setOutcome(outcome);
    } catch (DatabaseException &) {
        throw;
    }
}

void PurchaseSqlManager::addPurchaseTransaction(const QueryRequest &request, QueryResult &result, TransactionMode mode)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    const QVariantList &payments = params.value("payments").toList();
    const QVariantList &items = params.value("items").toList();
    int clientId = 0;
    int vendorId = 0;
    int purchaseTransactionNoteId = 0;
    int purchaseTransactionId = 0;
    int debtorId = 0;
    int debtTransactionId = 0;
    int creditorId = 0;
    int creditTransactionId = 0;

    QSqlQuery q(connection);

    try {
        //        AbstractSqlManager::enforceArguments( { "action" }, params);

        if (mode == TransactionMode::UseSqlTransaction)
            DatabaseUtils::beginTransaction(q);

        // STEP: Add client, if client does not exist.
        if (!params.value("customer_phone_number").toString().trimmed().isEmpty() && !params.value("suspended").toBool()) {
            QList<QSqlRecord> records(callProcedure("AddClientQuick", {
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

            records = callProcedure("AddVendor", {
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
                                    });
            vendorId = records.first().value("vendor_id").toInt();
        }

        // STEP: Insert note, if available.
        if (!params.value("note").toString().trimmed().isEmpty())
            purchaseTransactionNoteId = addNote(params.value("note", QVariant::String).toString(), QStringLiteral("purchase_transaction"));

        // STEP: Insert purchase transaction.
        const QList<QSqlRecord> &records(callProcedure("AddPurchaseTransaction", {
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
                                                               "note_id",
                                                               purchaseTransactionNoteId
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "user_id",
                                                               UserProfile::instance().userId()
                                                           }
                                                       }));

        purchaseTransactionId = records.first().value("purchase_transaction_id").toInt();

        // STEP: Insert purchase payments.
        for (const QVariant &payment : payments) {
            const QVariantMap &paymentInfo = payment.toMap();
            callProcedure("AddPurchasePayment", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "purchase_transaction_id",
                                  purchaseTransactionId
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
                          });
        }

        for (const QVariant &item : items) {
            QVariantMap itemInfo = item.toMap();
            // STEP: Deduct quantity if:
            // 1. This is a non-suspended transaction.
            // 2. This is a suspended transaction and you want to reserve the goods for this customer.
            if (!params.value("suspended", false).toBool()) {
                callProcedure("AddStockQuantity", {
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
                                  }
                              });
            }

            callProcedure("AddPurchaseItem", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "purchase_transaction_id",
                                  purchaseTransactionId
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
                          });
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
                                            "purchase_transaction"
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "transaction_id",
                                            purchaseTransactionId
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
                                            "note_id",
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
                                                            "note_id",
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
                                            creditorId
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "transaction_table",
                                            "purchase_transaction"
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "transaction_id",
                                            purchaseTransactionId
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
                                            "note_id",
                                            {}
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "user_id",
                                            UserProfile::instance().userId()
                                        }
                                    });
        }

        if (mode == TransactionMode::UseSqlTransaction)
            DatabaseUtils::commitTransaction(q);

        QVariantMap outcome;
        outcome.insert("client_id", clientId);
        outcome.insert("transaction_id", purchaseTransactionId);

        result.setOutcome(outcome);
    } catch (DatabaseException &) {
        if (mode == TransactionMode::UseSqlTransaction)
            DatabaseUtils::rollbackTransaction(q);

        throw;
    }
}

void PurchaseSqlManager::updateSuspendedTransaction(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        const QList<QSqlRecord> &records(callProcedure("IsPurchaseTransactionSuspended", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "transaction_id",
                                                               params.value("transaction_id")
                                                           }
                                                       }));

        if (!records.first().value("suspended").toBool())
            throw DatabaseException(DatabaseError::QueryErrorCode::UpdateTransactionFailure, QString(), "Transaction must be suspended.");

        addPurchaseTransaction(request, result, TransactionMode::SkipSqlTransaction);

        callProcedure("ArchivePurchaseTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_id",
                              params.value("transaction_id")
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

void PurchaseSqlManager::viewPurchaseCart(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QVariantMap params = request.params();
    QSqlQuery q(connection);

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewPurchaseCart", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "transaction_id",
                                                               params.value("transaction_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "purchase_transaction_archived",
                                                               params.value("purchase_transaction_archived", false)
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "purchase_item_archived",
                                                               params.value("purchase_item_archived", false)
                                                           }
                                                       }));

        QVariantMap outcome;
        QVariantList items;
        for (const QSqlRecord &record : records) {
            items.append(recordToMap(record));
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

void PurchaseSqlManager::undoAddPurchaseTransaction(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QVariantMap params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection);

    try {
        DatabaseUtils::beginTransaction(q);

        if (params.value("transaction_id").toInt() > 0) {
            callProcedure("ArchivePurchaseTransaction", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "archived",
                                  false
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "transaction_id",
                                  params.value("transaction_id")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  UserProfile::instance().userId()
                              }
                          });

            callProcedure("ArchiveDebtTransaction1", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "archived",
                                  false
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "transaction_table",
                                  QStringLiteral("purchase_transaction")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "transaction_id",
                                  params.value("transaction_id")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  UserProfile::instance().userId()
                              }
                          });

            callProcedure("ArchiveCreditTransaction", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "transaction_table",
                                  QStringLiteral("purchase_transaction")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "transaction_id",
                                  params.value("transaction_id")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  UserProfile::instance().userId()
                              }
                          });
        }

        callProcedure("RevertPurchaseQuantityUpdate", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_id",
                              params.value("transaction_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        DatabaseUtils::commitTransaction(q);
        result.setOutcome(request.params());
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void PurchaseSqlManager::viewPurchaseTransactions(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    QSqlQuery q(connection);

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewPurchaseTransactions", {
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

void PurchaseSqlManager::viewPurchaseTransactionItems(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "transaction_id" }, params);

        const QList<QSqlRecord> &records(callProcedure("ViewPurchaseTransactionItems", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "transaction_id",
                                                               params.value("transaction_id")
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "suspended",
                                                               params.value("suspended", false)
                                                           },
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "archived",
                                                               params.value("archived", false)
                                                           }
                                                       }));

        QVariantList items;
        for (const QSqlRecord &record : records) {
            items.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap { { "items", items }, { "record_count", items.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}

void PurchaseSqlManager::removePurchaseTransaction(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "transaction_id" }, params);

        DatabaseUtils::beginTransaction(q);

        callProcedure("ArchivePurchaseTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "archived",
                              true
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_id",
                              params.value("transaction_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        callProcedure("ArchiveDebtTransaction1", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "archived",
                              true
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_table",
                              QStringLiteral("purchase_transaction")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_id",
                              params.value("transaction_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        callProcedure("ArchiveCreditTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "archived",
                              true
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_table",
                              QStringLiteral("purchase_transaction")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_id",
                              params.value("transaction_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        callProcedure("RevertPurchaseQuantityUpdate", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_id",
                              params.value("transaction_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        DatabaseUtils::commitTransaction(q);
        result.setOutcome(QVariantMap { { "transaction_id", params.value("transaction_id").toInt() }, { "record_count", 1 } });
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void PurchaseSqlManager::undoRemovePurchaseTransaction(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "transaction_id" }, params);

        DatabaseUtils::beginTransaction(q);

        callProcedure("ArchivePurchaseTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "archived",
                              false
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_id",
                              params.value("transaction_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        callProcedure("ArchiveDebtTransaction1", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "archived",
                              false
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_table",
                              QStringLiteral("purchase_transaction")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_id",
                              params.value("transaction_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        callProcedure("ArchiveCreditTransaction", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_id",
                              params.value("transaction_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_table",
                              QStringLiteral("purchase_transaction")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_id",
                              params.value("transaction_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        callProcedure("UndoRevertPurchaseQuantityUpdate", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "transaction_id",
                              params.value("transaction_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        DatabaseUtils::commitTransaction(q);
        result.setOutcome(QVariantMap { { "transaction_id", params.value("transaction_id").toInt() }, { "record_count", 1 } });
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void PurchaseSqlManager::viewPurchaseReport(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();

    try {
        const QList<QSqlRecord> &records(callProcedure("ViewPurchaseReport", {
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
