#include "adddebtor.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace DebtorQuery;

AddDebtor::AddDebtor(const QString &preferredName,
                     const QString &firstName,
                     const QString &lastName,
                     const QString &primaryPhoneNumber,
                     const QUrl &imageUrl,
                     const DebtTransactionList &newDebtTransactions,
                     const QString &note,
                     QObject *receiver) :
    DebtorExecutor(COMMAND, {
                        { "can_undo", true },
                        { "preferred_name", preferredName },
                        { "first_name", firstName },
                        { "last_name", lastName },
                        { "image", DatabaseUtils::imageUrlToByteArray(imageUrl) },
                        { "primary_phone_number", primaryPhoneNumber },
                        { "new_debt_transactions", newDebtTransactions.toVariantList() },
                        { "note", note }
                   }, receiver)
{

}

QueryResult AddDebtor::execute()
{
    if (canUndo() && isUndoSet())
        return undoAddDebtor();

    return addDebtor();
}

QueryResult AddDebtor::addDebtor()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    const QVariantList &newDebtTransactions = params.value("new_debt_transactions").toList();
    int noteId = 0;
    int debtorId = 0;
    int clientId = 0;
    QVariantList debtTransactionIds;
    QVector<int> debtTransactionNoteIds;
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "preferred_name", "primary_phone_number" }, params);

        DatabaseUtils::beginTransaction(q);

        // STEP: Check if debt transactions exist
        if (newDebtTransactions.count() == 0)
            throw DatabaseException(DatabaseError::QueryErrorCode::MissingArguments,
                                    q.lastError().text(),
                                    QString("No new debt transactions for debtor %1.")
                                    .arg(params.value("preferred_name").toString()));

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
        return result;
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

QueryResult AddDebtor::undoAddDebtor()
{
    throw DatabaseException(DatabaseError::QueryErrorCode::NotYetImplementedError);
}
