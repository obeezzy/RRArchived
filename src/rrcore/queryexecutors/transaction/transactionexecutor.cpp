#include "transactionexecutor.h"
#include "database/exceptions/exceptions.h"
#include "user/userprofile.h"

TransactionExecutor::TransactionExecutor(const QString &command,
                                         const QVariantMap &params,
                                         const QueryRequest::QueryGroup &queryGroup,
                                         QObject *receiver) :
    QueryExecutor(command, params, queryGroup, receiver)
{

}

int TransactionExecutor::addDebtor(int clientId,
                                   const QString &note,
                                   const QString &tableName)
{
    const int noteId = TransactionExecutor::addNote(note,
                                                    tableName,
                                                    ExceptionPolicy::DisallowExceptions);
    const auto records(callProcedure("AddDebtor", {
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
                                     }));
    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral("Expected a debtor ID, received nothing."));

    return records.first().value("debtor_id").toInt();
}

int TransactionExecutor::addDebtTransaction(int debtorId,
                                            int transactionId,
                                            const QString &note,
                                            const QString &tableName)
{
    const int noteId = TransactionExecutor::addNote(note,
                                                    QStringLiteral("purchase_transaction"),
                                                    ExceptionPolicy::DisallowExceptions);

    const auto &records = callProcedure("AddDebtTransaction", {
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "debtor_id",
                                                debtorId
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "transaction_table",
                                                tableName
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "transaction_id",
                                                transactionId
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
        throw UnexpectedResultException(QStringLiteral("Expected debt transaction ID, received nothing."));

    return records.first().value("debt_transaction_id").toInt();
}

void TransactionExecutor::addDebtPayments(int debtTransactionId,
                                          const QString &note,
                                          const QString &tableName)
{
    const QVariantMap &params = request().params();
    const int noteId = TransactionExecutor::addNote(note,
                                                    tableName,
                                                    ExceptionPolicy::DisallowExceptions);

    callProcedure("AddDebtPayment", {
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
                          params.value("currency")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "due_date_time",
                          params.value("due_date_time")
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

int TransactionExecutor::addCreditor(int clientId,
                                     const QString &note,
                                     const QString &tableName)
{
    const int noteId = TransactionExecutor::addNote(note,
                                                    tableName,
                                                    ExceptionPolicy::DisallowExceptions);

    const auto &records(callProcedure("AddCreditor", {
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
                                      }));

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral("Expected creditor ID, received nothing."));

    return records.first().value("creditor_id").toInt();
}

int TransactionExecutor::addCreditTransaction(int creditorId,
                                              int transactionId,
                                              const QString &note,
                                              const QString &tableName)
{
    const int noteId = TransactionExecutor::addNote(note,
                                                    QStringLiteral("purchase_transaction"),
                                                    ExceptionPolicy::DisallowExceptions);

    const auto &records = callProcedure("AddCreditTransaction", {
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "creditor_id",
                                                creditorId
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "transaction_table",
                                                tableName
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "transaction_id",
                                                transactionId
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
        throw UnexpectedResultException(QStringLiteral("Expected credit transaction ID, received nothing."));

    return records.first().value("credit_transaction_id").toInt();
}

void TransactionExecutor::addCreditPayments(int creditTransactionId,
                                            const QString &note,
                                            const QString &tableName)
{
    const int noteId = TransactionExecutor::addNote(note,
                                                    tableName,
                                                    ExceptionPolicy::DisallowExceptions);
    const QVariantMap &params = request().params();

    callProcedure("AddCreditPayment", {
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
                          params.value("currency")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "due_date",
                          params.value("due_date")
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
