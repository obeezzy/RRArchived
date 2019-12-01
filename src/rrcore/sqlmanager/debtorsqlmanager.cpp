#include "debtorsqlmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

#include "user/userprofile.h"

DebtorSqlManager::DebtorSqlManager(const QString &connectionName) :
    AbstractSqlManager(connectionName)
{

}

QueryResult DebtorSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "add_new_debtor")
            addNewDebtor(request, result);
        else if (request.command() == "undo_add_new_debtor")
            undoAddNewDebtor(request, result);
        else if (request.command() == "update_debtor")
            updateDebtor(request, result);
        else if (request.command() == "view_debtors")
            viewDebtors(request, result);
        else if (request.command() == "remove_debtor")
            removeDebtor(request, result);
        else if (request.command() == "undo_remove_debtor")
            undoRemoveDebtor(request, result);
        else if (request.command() == "view_debt_transactions")
            viewDebtTransactions(request, result);
        else if (request.command() == "view_debtor_details")
            viewDebtorDetails(request, result);
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

void DebtorSqlManager::addNewDebtor(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    const QVariantList newDebtTransactions = params.value("new_debt_transactions").toList();
    int noteId = 0;
    int debtorId = 0;
    int clientId = 0;
    QVariantList debtTransactionIds;
    QVector<int> debtTransactionNoteIds;
    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "preferred_name", "primary_phone_number" }, params);

        DatabaseUtils::beginTransaction(q);

        // STEP: Check if debt transactions exist
        if (newDebtTransactions.count() == 0)
            throw DatabaseException(DatabaseError::QueryErrorCode::MissingArguments, q.lastError().text(),
                                    QString("No new debt transactions for debtor %1.").arg(params.value("preferred_name").toString()));

        // STEP: Ensure that debtor doesn't already exist
        if (!params.value("primary_phone_number").toString().isEmpty()) {
            const QList<QSqlRecord> records(callProcedure("IsExistingDebtor", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "phone_number",
                                                                  params.value("primary_phone_number")
                                                              }
                                                          }));

            if (records.first().value("id").toBool())
                throw DatabaseException(DatabaseError::QueryErrorCode::DuplicateEntryFailure, q.lastError().text(),
                                        "Failed to insert debtor because debtor already exists.");
        }

        noteId = addNote(params.value("note").toString(), "debtor");

        // STEP: Add notes for each debt transaction
        for (int i = 0; i < newDebtTransactions.count(); ++i) {
            const QString note = newDebtTransactions.at(i).toMap().value("note").toString();
            debtTransactionNoteIds.append(addNote(note, "debtor"));
        }

        if (newDebtTransactions.count() != debtTransactionNoteIds.count())
            throw DatabaseException(DatabaseError::QueryErrorCode::AddDebtorFailure, "",
                                    "Failed to match note ID count with transaction count.");

        // STEP: Add client
        QList<QSqlRecord> records(callProcedure("AddClient", {
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "first_name",
                                                        params.value("first_name")
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "last_name",
                                                        params.value("last_name")
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "preferred_name",
                                                        params.value("preferred_name")
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "phone_number",
                                                        params.value("primary_phone_number")
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "address",
                                                        params.value("address")
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "note_id",
                                                        noteId == 0 ? QVariant(QVariant::Int) : noteId
                                                    },
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "user_id",
                                                        UserProfile::instance().userId()
                                                    }
                                                }));

        clientId = records.first().value("id").toInt();

        // STEP: Add debtor
        records = callProcedure("AddDebtor", {
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "client_id",
                                        clientId
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "note_id",
                                        noteId == 0 ? QVariant(QVariant::Int) : noteId
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "user_id",
                                        UserProfile::instance().userId()
                                    }
                                });

        debtorId = records.first().value("id").toInt();

        // STEP: Add debt transactions
        for (int i = 0; i < newDebtTransactions.count(); ++i) {
            callProcedure("AddDebtTransaction", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "debtor_id",
                                  debtorId
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "transaction_table",
                                  QStringLiteral("debtor")
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "transaction_id",
                                  0
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "note_id",
                                  debtTransactionNoteIds.at(i) == 0 ? debtTransactionNoteIds.at(i) : QVariant(QVariant::Int)
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  UserProfile::instance().userId()
                              }
                          });

            const int debtTransactionId = records.first().value("id").toInt();
            const double totalDebt = newDebtTransactions.at(i).toMap().value("total_debt").toDouble();
            const QVariantList &newDebtPayments = newDebtTransactions.at(i).toMap().value("new_debt_payments").toList();
            const QDateTime &dueDateTime = newDebtTransactions.at(i).toMap().value("due_date").toDateTime();
            double newDebt = totalDebt;

            if (dueDateTime <= QDateTime::currentDateTime())
                throw DatabaseException(DatabaseError::QueryErrorCode::InvalidDueDate, q.lastError().text(),
                                        QStringLiteral("Due date is earlier than the current date."));

            debtTransactionIds.append(debtTransactionId);

            for (const QVariant &debtPayment : newDebtPayments) {
                const double amountPaid = debtPayment.toMap().value("amount").toDouble();
                const QString note = debtPayment.toMap().value("note").toString();

                const int noteId = !note.isEmpty() ? addNote(note, "debtor") : 0;

                // Add debt payment
                records = callProcedure("AddDebtPayment", {
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "debt_transaction_id",
                                                debtTransactionId
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "total_amount",
                                                newDebt
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "amount_paid",
                                                amountPaid
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "balance",
                                                qAbs(newDebt - amountPaid)
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "currency",
                                                QStringLiteral("NGN")
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "due_date",
                                                dueDateTime
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "note_id",
                                                noteId == 0 ? QVariant(QVariant::Int) : noteId
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "user_id",
                                                UserProfile::instance().userId()
                                            }
                                        });

                newDebt -= amountPaid;
                if (newDebt < 0.0)
                    throw DatabaseException(DatabaseError::QueryErrorCode::AmountOverpaid, q.lastError().text(),
                                            QStringLiteral("Total amount paid is greater than total debt."));
            }
        }

        result.setOutcome(QVariantMap {
                              { "client_id", clientId },
                              { "debtor_id", debtorId },
                              { "debt_transaction_ids", debtTransactionIds }
                          });

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void DebtorSqlManager::undoAddNewDebtor(const QueryRequest &request, QueryResult &result)
{
    Q_UNUSED(result)
    QSqlDatabase connection = QSqlDatabase::database(connectionName());

    const QVariantMap &params = request.params();
    const QVariantList &debtTransactionIds = params.value("outcome").toMap().value("debt_transaction_ids").toList();
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "client_id", "debtor_id", "debt_transaction_ids" }, params.value("outcome").toMap());

        DatabaseUtils::beginTransaction(q);

        // Remove debtor
        callProcedure("UndoArchiveDebtor", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "debtor_id",
                              params.value("outcome").toMap().value("debtor_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        // Remove debt transaction
        for (const QVariant &debtTransactionId : debtTransactionIds) {
            callProcedure("UndoArchiveDebtTransaction", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "debt_transaction_id",
                                  debtTransactionId
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  UserProfile::instance().userId()
                              }
                          });

            // Remove debt payment for debt transaction ID
            callProcedure("UndoArchiveDebtPayment", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "debt_transaction_id",
                                  debtTransactionId
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  UserProfile::instance().userId()
                              }
                          });
        }

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void DebtorSqlManager::updateDebtor(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    const QVariantList &updatedDebtTransactions = params.value("updated_debt_transactions").toList();
    const QVariantList &newDebtTransactions = params.value("new_debt_transactions").toList();
    const QVariantList &removedDebtTransactionIds = params.value("removed_debt_transaction_ids").toList();
    const QVariantList &removedDebtPaymentIds = params.value("removed_debt_payment_ids").toList();
    int noteId = params.value("note_id").toInt();
    const QString &note = params.value("note").toString();
    int debtorId = params.value("debtor_id").toInt();
    int clientId = params.value("client_id").toInt();
    QVariantList updatedDebtTransactionIds;
    QVariantList newDebtTransactionIds;
    QVector<int> newDebtTransactionNoteIds;
    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "client_id", "debtor_id", "preferred_name", "primary_phone_number" }, params);

        DatabaseUtils::beginTransaction(q);

        // STEP: Check if debt transactions exist.
        if (updatedDebtTransactions.isEmpty() && newDebtTransactions.isEmpty())
            throw DatabaseException(DatabaseError::QueryErrorCode::MissingArguments, q.lastError().text(),
                                    QString("No new/updated debt transactions for debtor %1.").arg(params.value("preferred_name").toString()));

        // STEP: Update notes for debtor.
        AbstractSqlManager::updateNote(noteId, note, "debtor");

        // STEP: Add notes for updated debt transaction.
        for (int i = 0; i < updatedDebtTransactions.count(); ++i) {
            const int noteId = updatedDebtTransactions.at(i).toMap().value("note_id").toInt();
            const QString &note = updatedDebtTransactions.at(i).toMap().value("note").toString();
            AbstractSqlManager::updateNote(noteId, note, "debtor");
        }

        // STEP: Add notes for new debt transaction.
        for (int i = 0; i < newDebtTransactions.count(); ++i) {
            const QString &note = newDebtTransactions.at(i).toMap().value("note").toString();
            newDebtTransactionNoteIds.append(AbstractSqlManager::addNote(note, "debtor"));
        }

        if (newDebtTransactions.count() != newDebtTransactionNoteIds.count())
            throw DatabaseException(DatabaseError::QueryErrorCode::UpdateDebtorFailure, "",
                                    QStringLiteral("Failed to match note ID count with transaction count."));

        // STEP: Update client details.
        callProcedure("UpdateClient", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "client_id",
                              clientId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "first_name",
                              params.value("first_name")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "last_name",
                              params.value("last_name")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "preferred_name",
                              params.value("preferred_name")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "phone_number",
                              params.value("primary_phone_number")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        // STEP: Update debt transactions.
        for (int i = 0; i < updatedDebtTransactions.count(); ++i) {
            const int debtTransactionId = updatedDebtTransactions.at(i).toMap().value("debt_transaction_id").toInt();
            const double totalDebt = updatedDebtTransactions.at(i).toMap().value("total_debt").toDouble();
            const QDateTime &dueDateTime = updatedDebtTransactions.at(i).toMap().value("due_date").toDateTime();
            double newDebt = totalDebt;

            if (debtTransactionId <= 0)
                throw DatabaseException(DatabaseError::QueryErrorCode::InvalidArguments, q.lastError().text(),
                                        QStringLiteral("Debt transaction ID used to update table is invalid."));

            if (dueDateTime.isNull() || !dueDateTime.isValid() || dueDateTime <= QDateTime::currentDateTime())
                throw DatabaseException(DatabaseError::QueryErrorCode::InvalidDueDate, q.lastError().text(),
                                        QStringLiteral("Due date is earlier than the current date or invalid."));

            // Update timestamp for debt transactions.
            callProcedure("TouchDebtTransaction", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "debt_transaction_id",
                                  debtTransactionId
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  UserProfile::instance().userId()
                              }
                          });

            // STEP: Update debt payments.
            const QVariantList &updatedDebtPayments(updatedDebtTransactions.at(i).toMap().value("updated_debt_payments").toList());
            for (int j = 0; j < updatedDebtPayments.count(); ++j) {
                const int debtPaymentId = updatedDebtPayments.at(j).toMap().value("debt_payment_id").toInt();
                const double amountPaid = updatedDebtPayments.at(j).toMap().value("amount_paid").toDouble();

                if (debtPaymentId <= 0)
                    throw DatabaseException(DatabaseError::QueryErrorCode::InvalidArguments, q.lastError().text(),
                                            QStringLiteral("Debt payment ID used to update table is invalid."));

                callProcedure("UpdateDebtPayment", {
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "debt_payment_id",
                                      debtPaymentId
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "total_amount",
                                      newDebt
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "amount_paid",
                                      amountPaid
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "balance",
                                      qAbs(newDebt - amountPaid)
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "currency",
                                      QStringLiteral("NGN")
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "due_date",
                                      dueDateTime
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "user_id",
                                      UserProfile::instance().userId()
                                  }
                              });

                newDebt -= amountPaid;
                if (newDebt < 0.0)
                    throw DatabaseException(DatabaseError::QueryErrorCode::AmountOverpaid, q.lastError().text(),
                                            QStringLiteral("Total amount paid is greater than total debt."));
            }

            // STEP: Add new debt payments.
            const QVariantList &newDebtPayments(updatedDebtTransactions.at(i).toMap().value("new_debt_payments").toList());
            for (int j = 0; j < newDebtPayments.count(); ++j) {
                const double updatedDebt = newDebtPayments.at(j).toMap().value("total_amount").toDouble();
                const double amountPaid = newDebtPayments.at(j).toMap().value("amount_paid").toDouble();

                callProcedure("AddDebtPayment", {
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "debt_transaction_id",
                                      debtTransactionId
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "total_amount",
                                      updatedDebt
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "amount_paid",
                                      amountPaid
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "balance",
                                      qAbs(updatedDebt - amountPaid)
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "currency",
                                      QStringLiteral("NGN")
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "due_date",
                                      dueDateTime
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "note_id",
                                      noteId == 0 ? QVariant(QVariant::Int) : noteId
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "user_id",
                                      UserProfile::instance().userId()
                                  }
                              });
            }
        }

        // STEP: Add debt transactions
        for (int i = 0; i < newDebtTransactions.count(); ++i) {
            const QList<QSqlRecord> records(callProcedure("AddDebtTransaction", {
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "debtor_id",
                                                                  debtorId
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "transaction_table",
                                                                  QStringLiteral("debtor")
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "transaction_id",
                                                                  0
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "note_id",
                                                                  newDebtTransactionNoteIds.at(i) == 0 ?
                                                                  QVariant(QVariant::Int) : newDebtTransactionNoteIds.at(i)
                                                              },
                                                              ProcedureArgument {
                                                                  ProcedureArgument::Type::In,
                                                                  "user_id",
                                                                  UserProfile::instance().userId()
                                                              }
                                                          }));

            const int debtTransactionId = records.first().value("id").toInt();
            const double totalDebt = newDebtTransactions.at(i).toMap().value("total_debt").toDouble();
            const QVariantList newDebtPayments = newDebtTransactions.at(i).toMap().value("new_debt_payments").toList();
            const QDateTime dueDateTime = newDebtTransactions.at(i).toMap().value("due_date").toDateTime();
            double newDebt = totalDebt;

            if (dueDateTime.isNull() || !dueDateTime.isValid() || dueDateTime <= QDateTime::currentDateTime())
                throw DatabaseException(DatabaseError::QueryErrorCode::InvalidDueDate, q.lastError().text(),
                                        QStringLiteral("Due date is earlier than the current date or invalid."));

            newDebtTransactionIds.append(debtTransactionId);

            for (const QVariant &debtPayment : newDebtPayments) {
                const double amountPaid = debtPayment.toMap().value("amount").toDouble();
                const QString &note = debtPayment.toMap().value("note").toString();

                const int noteId = !note.isEmpty() ? addNote(note, "debtor") : 0;

                // Add debt payment
                callProcedure("AddDebtPayment", {
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "debt_transaction_id",
                                      debtTransactionId
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "total_amount",
                                      newDebt
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "amount_paid",
                                      amountPaid
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "balance",
                                      qAbs(newDebt - amountPaid)
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "currency",
                                      QStringLiteral("NGN")
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "due_date",
                                      dueDateTime
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "note_id",
                                      noteId == 0 ? QVariant(QVariant::Int) : noteId
                                  },
                                  ProcedureArgument {
                                      ProcedureArgument::Type::In,
                                      "user_id",
                                      UserProfile::instance().userId()
                                  }
                              });

                newDebt -= amountPaid;
                if (newDebt < 0.0)
                    throw DatabaseException(DatabaseError::QueryErrorCode::AmountOverpaid, q.lastError().text(),
                                            QStringLiteral("Total amount paid is greater than total debt."));
            }
        }

        // STEP: Remove debt transactions.
        for (int i = 0; i < removedDebtTransactionIds.count(); ++i) {
            callProcedure("ArchiveDebtTransaction2", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "debt_transaction_id",
                                  removedDebtTransactionIds.at(i)
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  UserProfile::instance().userId()
                              }
                          });
        }

        // STEP: Remove debt payments.
        for (int i = 0; i < removedDebtPaymentIds.count(); ++i) {
            callProcedure("ArchiveDebtPayment", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "debt_payment_id",
                                  removedDebtPaymentIds.at(i)
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  UserProfile::instance().userId()
                              }
                          });
        }

        result.setOutcome(QVariantMap {
                              { "client_id", clientId },
                              { "debtor_id", debtorId },
                              { "updated_debt_transaction_ids", updatedDebtTransactionIds },
                              { "new_debt_transaction_ids", newDebtTransactionIds }
                          });

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);

        throw;
    }
}

void DebtorSqlManager::viewDebtors(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    QList<QSqlRecord> records;
    QSqlQuery q(connection);

    try {
        // STEP: Get total balance for each debtor.
        if (params.value("filter_text").isNull() || params.value("filter_column").isNull()) {
            records = callProcedure("ViewDebtors", {
                                        ProcedureArgument {
                                            ProcedureArgument::Type::In,
                                            "archived",
                                            params.value("archived", false)
                                        }
                                    });
        } else {
            // STEP: Filter total balance for each debtor
            if (params.value("filter_column").toString() == "preferred_name") {
                records = callProcedure("FilterDebtorsByName", {
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "name",
                                                params.value("filter_text")
                                            },
                                            ProcedureArgument {
                                                ProcedureArgument::Type::In,
                                                "archived",
                                                params.value("archived")
                                            }
                                        });
            }
        }

        QVariantList debtors;
        for (const QSqlRecord &record : records)
            debtors.append(recordToMap(record));

        result.setOutcome(QVariantMap {
                              { "debtors", debtors },
                              { "record_count", debtors.count() }
                          });
    } catch (DatabaseException &) {
        throw;
    }
}

void DebtorSqlManager::removeDebtor(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "debtor_id" }, params);

        if (params.value("debtor_id").toInt() <= 0)
            throw DatabaseException(DatabaseError::QueryErrorCode::InvalidArguments, QString(),
                                    QStringLiteral("Debtor ID is invalid."));

        DatabaseUtils::beginTransaction(q);

        callProcedure("ArchiveDebtor", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "debtor_id",
                              params.value("debtor_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        // Archive debt transactions.
        callProcedure("ArchiveDebtTransaction3", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "debtor_id",
                              params.value("debtor_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        // Retrieve archived debt transaction IDs.
        const QList<QSqlRecord> records(callProcedure("GetDebtTransaction", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "debtor_id",
                                                              params.value("debtor_id")
                                                          }
                                                      }));

        QVariantList debtTransactionIds;
        for (const QSqlRecord &record : records) {
            debtTransactionIds.append(record.value("id").toInt());
        }

        result.setOutcome(QVariantMap{ { "debtor_id", params.value("debtor_id") },
                                       { "debt_transaction_ids", debtTransactionIds },
                                       { "record_count", QVariant(1) },
                                       { "debtor_row", params.value("debtor_row") }
                          });

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void DebtorSqlManager::undoRemoveDebtor(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    const QVariantList &debtTransactionIds = params.value("outcome").toMap().value("debt_transaction_ids").toList();

    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "debtor_id" }, params);

        if (params.value("debtor_id").toInt() <= 0)
            throw DatabaseException(DatabaseError::QueryErrorCode::InvalidArguments, QString(), "Debtor ID is invalid.");

        DatabaseUtils::beginTransaction(q);

        // Archive debt transactions.
        callProcedure("UndoArchiveDebtor", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "debtor_id",
                              params.value("debtor_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        // STEP: Restore archived transactions.
        for (int i = 0; i < debtTransactionIds.count(); ++i) {
            callProcedure("UndoArchiveDebtTransaction", {
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "debt_transaction_id",
                                  debtTransactionIds.at(i).toInt()
                              },
                              ProcedureArgument {
                                  ProcedureArgument::Type::In,
                                  "user_id",
                                  UserProfile::instance().userId()
                              }
                          });
        }

        // STEP: Get total balance for each debtor.
        const QList<QSqlRecord> records(callProcedure("ViewTotalBalanceForDebtor", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "debtor_id",
                                                              params.value("debtor_id")
                                                          }
                                                      }));

        QVariantMap debtor;
        if (!records.isEmpty()) {
            debtor = recordToMap(records.first());
            result.setOutcome(QVariantMap {
                                  { "debtor", QVariant(debtor) },
                                  { "record_count", 1 },
                                  { "debtor_id", params.value("debtor_id") },
                                  { "debtor_row", params.value("debtor_row") }
                              });
        } else {
            throw DatabaseException(DatabaseError::QueryErrorCode::UndoRemoveDebtorFailure,
                                    q.lastError().text(),
                                    QStringLiteral("Unable to fetch removed debtor."));
        }

        DatabaseUtils::commitTransaction(q);
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void DebtorSqlManager::viewDebtTransactions(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    int clientId = 0;
    QString preferredName;
    QString primaryPhoneNumber;
    QString debtorNote;
    QSqlQuery q(connection);
    QList<QSqlRecord> records;

    try {
        AbstractSqlManager::enforceArguments({ "debtor_id" }, params);

        records = callProcedure("ViewFewDebtorDetails", {
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "debtor_id",
                                        params.value("debtor_id")
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "archived",
                                        params.value("archived", false)
                                    }
                                });

        if (!records.isEmpty()) {
            clientId = records.first().value("client_id").toInt();
            preferredName = records.first().value("preferred_name").toString();
            primaryPhoneNumber = records.first().value("primary_phone_number").toString();
            debtorNote = records.first().value("note").toString();
        }

        records = callProcedure("ViewDebtTransactions", {
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "debtor_id",
                                        params.value("debtor_id")
                                    },
                                    ProcedureArgument {
                                        ProcedureArgument::Type::In,
                                        "archived",
                                        params.value("archived", false)
                                    }
                                });

        QVariantList transactions;
        QVariantList paymentGroups;

        for (int i = 0; i < records.count(); ++i) {
            QSqlRecord &record = records[i];
            QVariantMap transactionRecord;
            const int debtTransactionId = record.value("debt_transaction_id").toInt();
            const QString &relatedTransactionTable = record.value("related_transaction_table").toString();
            const int relatedTransactionId = record.value("related_transaction_id").toInt();
            const double totalDebt = record.value("balance").toDouble();
            const QDateTime &dueDate = record.value("due_date").toDateTime();
            const QDateTime created = record.value("debt_transaction_created").toDateTime();

            transactionRecord.insert("debt_transaction_id", debtTransactionId);
            transactionRecord.insert("related_transaction_table", relatedTransactionTable);
            transactionRecord.insert("related_transaction_id", relatedTransactionId);
            transactionRecord.insert("total_debt", totalDebt);
            transactionRecord.insert("note_id", record.value("debt_transaction_note_id"));
            transactionRecord.insert("due_date", dueDate);
            transactionRecord.insert("created", created);

            QVariantList payments;

            do {
                QVariantMap paymentRecord;
                paymentRecord.insert("debt_transaction_id", debtTransactionId);
                paymentRecord.insert("debt_payment_id", record.value("debt_payment_id"));
                paymentRecord.insert("total_amount", record.value("total_amount"));
                paymentRecord.insert("amount_paid", record.value("amount_paid"));
                paymentRecord.insert("balance", record.value("balance"));
                paymentRecord.insert("currency", record.value("currency"));
                paymentRecord.insert("due_date", record.value("due_date"));
                paymentRecord.insert("note_id", record.value("debt_payment_note_id"));
                paymentRecord.insert("archived", record.value("archived"));
                paymentRecord.insert("created", record.value("debt_payment_created"));

                payments.append(paymentRecord);
            } while ((++i < records.count()) && debtTransactionId == record.value("debt_transaction_id").toInt());

            transactions.append(transactionRecord);
            paymentGroups.append(QVariant(payments));
            --i;
        }

        if (transactions.count() != paymentGroups.count())
            throw DatabaseException(DatabaseError::QueryErrorCode::ResultMismatch,
                                    QString("Transaction count (%1) and payment group count (%2) are unequal.")
                                    .arg(transactions.count()).arg(paymentGroups.count()));

        result.setOutcome(QVariantMap {
                              { "client_id", clientId },
                              { "debtor_id", params.value("debtor_id") },
                              { "preferred_name", preferredName },
                              { "primary_phone_number", primaryPhoneNumber },
                              { "note", debtorNote },
                              { "transactions", transactions },
                              { "payment_groups", paymentGroups },
                              { "record_count", transactions.count() }
                          });
    } catch (DatabaseException &) {
        throw;
    }
}

void DebtorSqlManager::viewDebtorDetails(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    QSqlQuery q(connection);

    try {
        AbstractSqlManager::enforceArguments({ "debtor_id" }, params);

        // STEP: Get total balance for each debtor
        const auto records = callProcedure("ViewDebtorDetails", {
                                               ProcedureArgument {
                                                   ProcedureArgument::Type::In,
                                                   "debtor_id",
                                                   params.value("debtor_id")
                                               },
                                               ProcedureArgument {
                                                   ProcedureArgument::Type::In,
                                                   "archived",
                                                   params.value("archived", false)
                                               }
                                           });

        if (records.isEmpty())
            throw DatabaseException(DatabaseError::QueryErrorCode::InvalidArguments,
                                    QString(),
                                    QStringLiteral("Debtor with ID %1 does not exist.")
                                    .arg(params.value("debtor_id").toInt()));

        result.setOutcome(QVariantMap {
                              { "debtor", recordToMap(records.first()) },
                              { "record_count", 1 }
                          });
    } catch (DatabaseException &) {
        throw;
    }
}
