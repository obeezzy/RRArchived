#include "updatedebtor.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace DebtorQuery;

UpdateDebtor::UpdateDebtor(int debtorId,
                           int clientId,
                           const QString &preferredName,
                           const QString &firstName,
                           const QString &lastName,
                           const QString &primaryPhoneNumber,
                           const QUrl &imageUrl,
                           const DebtTransactionList &newDebtTransactions,
                           const DebtTransactionList &updatedDebtTransactions,
                           const QString &note,
                           QObject *receiver) :
    DebtorExecutor(COMMAND, {
                        { "can_undo", true },
                        { "debtor_id", debtorId },
                        { "client_id", clientId },
                        { "preferred_name", preferredName },
                        { "first_name", firstName },
                        { "last_name", lastName },
                        { "image", DatabaseUtils::imageUrlToByteArray(imageUrl) },
                        { "primary_phone_number", primaryPhoneNumber },
                        { "new_debt_transactions", newDebtTransactions.toVariantList() },
                        { "updated_debt_transactions", updatedDebtTransactions.toVariantList() },
                        { "note", note }
                   }, receiver)
{

}

QueryResult UpdateDebtor::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
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
        QueryExecutor::enforceArguments({ "client_id", "debtor_id", "preferred_name", "primary_phone_number" }, params);

        DatabaseUtils::beginTransaction(q);

        // STEP: Check if debt transactions exist.
        if (updatedDebtTransactions.isEmpty() && newDebtTransactions.isEmpty())
            throw DatabaseException(DatabaseError::QueryErrorCode::MissingArguments, q.lastError().text(),
                                    QString("No new/updated debt transactions for debtor %1.").arg(params.value("preferred_name").toString()));

        // STEP: Update notes for debtor.
        QueryExecutor::updateNote(noteId, note, "debtor");

        // STEP: Add notes for updated debt transaction.
        for (int i = 0; i < updatedDebtTransactions.count(); ++i) {
            const int noteId = updatedDebtTransactions.at(i).toMap().value("note_id").toInt();
            const QString &note = updatedDebtTransactions.at(i).toMap().value("note").toString();
            QueryExecutor::updateNote(noteId, note, "debtor");
        }

        // STEP: Add notes for new debt transaction.
        for (int i = 0; i < newDebtTransactions.count(); ++i) {
            const QString &note = newDebtTransactions.at(i).toMap().value("note").toString();
            newDebtTransactionNoteIds.append(QueryExecutor::addNote(note, "debtor"));
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
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}
