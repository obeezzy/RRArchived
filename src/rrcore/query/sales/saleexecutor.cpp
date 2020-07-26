#include "saleexecutor.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"
#include "database/exceptions/exceptions.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

SaleExecutor::SaleExecutor(const QString &command,
                           const QVariantMap &params,
                           QObject *receiver) :
    TransactionExecutor(command, params, QueryRequest::QueryGroup::Sales, receiver)
{}

QueryResult SaleExecutor::addSaleTransaction(TransactionMode mode)
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();
    const QString &clientPhoneNumber = params.value("client_phone_number").toString().trimmed();
    const double balance = params.value("balance").toDouble();
    const bool suspended = params.value("suspended").toBool();
    const bool overlookBalance = params.value("overlook_balance").toBool();
    int clientId = 0;
    int customerId = 0;
    int saleTransactionId = 0;
    int debtorId = 0;
    int debtTransactionId = 0;
    int creditorId = 0;
    int creditTransactionId = 0;

    Q_UNUSED(customerId)

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments( { "action" }, params);

        if (mode == TransactionMode::UseSqlTransaction)
            QueryExecutor::beginTransaction(q);

        if (!clientPhoneNumber.isEmpty() && !suspended) {
            clientId = addClient();
            customerId = addCustomer(clientId);
        }

        saleTransactionId = addSaleTransactionToDatabase(clientId);
        addSalePayments(saleTransactionId);
        addSoldProducts(saleTransactionId);

        if (!overlookBalance && !suspended && balance > 0.0) {
            debtorId = TransactionExecutor::addDebtor(clientId);
            debtTransactionId = TransactionExecutor::addDebtTransaction(debtorId,
                                                                        saleTransactionId);
            TransactionExecutor::addDebtPayments(debtTransactionId);
        } else if (!overlookBalance && !suspended && balance < 0.0) {
            creditorId = TransactionExecutor::addCreditor(clientId);
            creditTransactionId = TransactionExecutor::addCreditTransaction(creditorId,
                                                                            saleTransactionId);
            TransactionExecutor::addCreditPayments(creditTransactionId);
        }

        if (mode == TransactionMode::UseSqlTransaction)
            QueryExecutor::commitTransaction(q);

        result.setOutcome(QVariantMap {
                              { "client_id", clientId },
                              { "sale_transaction_id", saleTransactionId }
                          });
        return result;
    } catch (const DatabaseException &) {
        if (mode == TransactionMode::UseSqlTransaction)
            QueryExecutor::rollbackTransaction(q);

        throw;
    }
}

int SaleExecutor::addClient()
{
    const QVariantMap &params{ request().params() };

    const auto &records(callProcedure("AddClientLite", {
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "preferred_name",
                                              params.value("client_preferred_name")
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "phone_number",
                                              params.value("client_phone_number")
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "user_id",
                                              UserProfile::instance().userId()
                                          }
                                      }));

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral("Expected a client ID, received nothing."));

    return records.first().value("client_id").toInt();
}

int SaleExecutor::addCustomer(int clientId)
{
    const QVariantMap &params{ request().params() };
    const QString &note = params.value("note").toString();
    const int noteId = QueryExecutor::addNote(note,
                                              QStringLiteral("sale_transaction"),
                                              ExceptionPolicy::DisallowExceptions);
    const auto &records = callProcedure("AddCustomer", {
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "client_id",
                                                clientId
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
        throw UnexpectedResultException(QStringLiteral("Expected customer ID but received nothing."));

    return records.first().value("customer_id").toInt();
}

int SaleExecutor::addSaleTransactionToDatabase(int clientId)
{
    const QVariantMap &params{ request().params() };
    const bool shouldGiveChange = params.value("action").toString() == QStringLiteral("give_change");
    const double balance = shouldGiveChange ? 0.0
                                           : qAbs(params.value("balance").toDouble());
    const QString &note = params.value("note").toString();
    const int noteId = QueryExecutor::addNote(note,
                                              QStringLiteral("sale_transaction"),
                                              ExceptionPolicy::DisallowExceptions);

    const auto &records(callProcedure("AddSaleTransaction", {
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "name",
                                              params.value("client_preferred_name")
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "client_id",
                                              clientId
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "balance",
                                              balance
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
                                              noteId
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "user_id",
                                              UserProfile::instance().userId()
                                          }
                                      }));
    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral("Expected sale transaction ID, received nothing."));

    return records.first().value("sale_transaction_id").toInt();
}

void SaleExecutor::addSalePayments(int saleTransactionId)
{
    const QVariantMap &params{ request().params() };
    const QVariantList &payments = params.value("payments").toList();

    for (const QVariant &paymentAsVariant : payments) {
        const QVariantMap &payment = paymentAsVariant.toMap();
        const QString &note = params.value("note").toString();
        const int noteId = QueryExecutor::addNote(note,
                                                  QStringLiteral("sale_transaction"),
                                                  ExceptionPolicy::DisallowExceptions);

        callProcedure("AddSalePayment", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "sale_transaction_id",
                              saleTransactionId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "amount",
                              payment.value("amount")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "payment_method",
                              payment.value("payment_method")
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
}

void SaleExecutor::addSoldProducts(int saleTransactionId)
{
    const QVariantMap &params{ request().params() };
    const QVariantList &products = params.value("products").toList();
    const bool suspended = params.value("suspended").toBool();

    for (const QVariant &productAsVariant : products) {
        const QVariantMap &product = productAsVariant.toMap();
        if (!suspended)
            deductFromStockProductQuantity(product);

        callProcedure("AddSaleProduct", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "sale_transaction_id",
                              saleTransactionId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "product_id",
                              product.value("product_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "product_unit_id",
                              product.value("product_unit_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "unit_price",
                              product.value("unit_price")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "quantity",
                              product.value("quantity")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "cost",
                              product.value("cost")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "discount",
                              product.value("discount")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "currency",
                              product.value("currency")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });
    }
}

void SaleExecutor::deductFromStockProductQuantity(const QVariantMap &product)
{
    // STEP: Deduct quantity if:
    // 1. This is a non-suspended transaction.
    // 2. This is a suspended transaction and you want to reserve the goods for this customer.
    const QString &reason = request().command();

    callProcedure("DeductStockQuantity", {
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "product_id",
                          product.value("product_id")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "quantity",
                          product.value("quantity").toDouble()
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "product_unit_id",
                          product.value("product_unit_id")
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
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::Out,
                          "initial_quantity_id",
                          {}
                      }
                  });
}
