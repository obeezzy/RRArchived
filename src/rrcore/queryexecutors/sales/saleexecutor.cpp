#include "saleexecutor.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

SaleExecutor::SaleExecutor(const QString &command,
                           const QVariantMap &params,
                           QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::QueryGroup::Sales, receiver)
{

}

QueryResult SaleExecutor::addSaleTransaction(QueryExecutor::TransactionMode mode)
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    const QVariantList &payments = params.value("payments").toList();
    const QVariantList &items = params.value("items").toList();
    int clientId = 0;
    int saleTransactionId = 0;
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
            const QList<QSqlRecord> records(callProcedure("AddClientQuick", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "preferred_name",
                                                                  params.value("customer_name")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "phone_number",
                                                                  params.value("customer_phone_number")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  params.value("user_id")
                                                              }
                                                          }));

            clientId = records.first().value("id").toInt();
        }

        // STEP: Insert note, if available.
        if (!params.value("note").toString().trimmed().isEmpty()) {
            addNote(params.value("note", QVariant::String).toString(),
                    QStringLiteral("sale_transaction"));
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
                                                                  paymentInfo.value("method")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "currency",
                                                                  params.value("currency")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "note",
                                                                  params.value("note")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  params.value("user_id")
                                                              }
                                                          }));
        }

        for (const QVariant &item : items) {
            QVariantMap itemInfo = item.toMap();
            // STEP: Deduct quantity if:
            // 1. This is a non-suspended transaction.
            // 2. This is a suspended transaction and you want to reserve the goods for this customer.
            if (!params.value("suspended", false).toBool()) {
                callProcedure("DeductStockQuantity", {
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
                                      request().command()
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
                              });
            }

            callProcedure("AddSaleItem", {
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
                                            "sale_transaction"
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "transaction_id",
                                            saleTransactionId
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
        } else if (!params.value("overlook_balance").toBool()
                   && !params.value("suspended").toBool()
                   && params.value("balance").toDouble() < 0.0) {
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
                                                            params.value("user_id")
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
                                            "sale_transaction"
                                        },
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "transaction_id",
                                            saleTransactionId
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

        result.setOutcome(QVariantMap {
                              { "client_id", clientId },
                              { "transaction_id", saleTransactionId }
                          });
        return result;
    } catch (DatabaseException &) {
        if (mode == TransactionMode::UseSqlTransaction)
            DatabaseUtils::rollbackTransaction(q);

        throw;
    }
}
