#include "adddebtor.h"
#include "database/databaseutils.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"
#include "database/exceptions/exceptions.h"
#include "singletons/settings.h"
#include "utility/debtorutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace DebtorQuery;

AddDebtor::AddDebtor(const Utility::Debtor &debtor,
                     QObject *receiver) :
    DebtorExecutor(COMMAND, {
                    { "can_undo", true },
                    { "preferred_name", debtor.client.preferredName },
                    { "first_name", debtor.client.firstName },
                    { "last_name", debtor.client.lastName },
                    { "image_url", debtor.client.imageUrl },
                    { "phone_number", debtor.client.phoneNumber },
                    { "note_id", debtor.note.id },
                    { "note", debtor.note.note }
                   }, receiver)
{
    DebtorExecutor::arrangeDebtTransactions(debtor.debtTransactions);
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

    const QVariantMap &params = request().params();
    const QString &phoneNumber = request().params().value("phone_number").toString();
    const QVariantList &newDebtTransactions = params.value("new_debt_transactions").toList();
    int noteId = 0;
    int debtorId = 0;
    int clientId = 0;
    QVariantList debtTransactionIds;
    QVariantList debtPaymentIds;
    QVariantList debtTransactionNoteIds;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "preferred_name",
                                          "phone_number"
                                        }, params);

        if (newDebtTransactions.count() == 0)
            throw MissingArgumentException(QStringLiteral("No new debt transactions for debtor %1.")
                                           .arg(params.value("preferred_name").toString()));

        DatabaseUtils::beginTransaction(q);

        affirmDebtorDoesNotExist(phoneNumber);
        noteId = addNotesForDebtor(params,
                                   newDebtTransactions,
                                   debtTransactionNoteIds);
        clientId = addClient(params,
                             noteId);
        addDebtorToDatabase(clientId,
                            noteId);
        addDebtTransactions(debtorId,
                            newDebtTransactions,
                            debtTransactionNoteIds,
                            debtTransactionIds,
                            debtPaymentIds);

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
    throw MissingImplementationException();
}

void AddDebtor::affirmDebtorDoesNotExist(const QString &phoneNumber)
{
    if (!phoneNumber.isEmpty()) {
        const QList<QSqlRecord> &records(callProcedure("IsExistingDebtor", {
                                                           ProcedureArgument {
                                                               ProcedureArgument::Type::In,
                                                               "phone_number",
                                                               phoneNumber
                                                           }
                                                       }));

        if (records.first().value("debtor_id").toBool())
            throw DuplicateEntryException(QStringLiteral("Debtor already exists."));
    }
}

int AddDebtor::addNotesForDebtor(const QVariantMap &params,
                                 const QVariantList &newDebtTransactions,
                                 QVariantList &debtTransactionNoteIds)
{
    for (const auto &transactionAsVariant : newDebtTransactions) {
        const QString &note = transactionAsVariant.toMap().value("note").toString();
        debtTransactionNoteIds.append(QueryExecutor::addNote(note,
                                                             QStringLiteral("debtor")));
    }

    if (newDebtTransactions.count() != debtTransactionNoteIds.count())
        throw ArgumentMismatchException(QStringLiteral("Failed to match note ID count with transaction count."));

    return QueryExecutor::addNote(params.value("note").toString(),
                                  QStringLiteral("debtor"));
}

int AddDebtor::addClient(const QVariantMap &params,
                         int noteId)
{
    const QByteArray &imageBlob = DatabaseUtils::imageUrlToByteArray(params.value("image_url").toUrl());
    Q_UNUSED(imageBlob)
    const auto &records(callProcedure("AddClient", {
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
                                              params.value("phone_number")
                                          },
                                          ProcedureArgument {
                                              ProcedureArgument::Type::In,
                                              "address",
                                              params.value("address")
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

    return records.first().value("client_id").toInt();
}

int AddDebtor::addDebtorToDatabase(int clientId,
                                   int noteId)
{
    const auto &records = callProcedure("AddDebtor", {
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

    return records.first().value("debtor_id").toInt();
}

void AddDebtor::addDebtTransactions(int debtorId,
                                    const QVariantList &newDebtTransactions,
                                    const QVariantList &debtTransactionNoteIds,
                                    QVariantList &newDebtTransactionIds,
                                    QVariantList &newDebtPaymentIds)
{
    for (int i = 0; i < newDebtTransactions.count(); ++i) {
        const auto &records = callProcedure("AddDebtTransaction", {
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
                                                    debtTransactionNoteIds.at(i)
                                                },
                                                ProcedureArgument {
                                                    ProcedureArgument::Type::In,
                                                    "user_id",
                                                    UserProfile::instance().userId()
                                                }
                                            });

        const int debtTransactionId = records.first().value("debt_transaction_id").toInt();
        newDebtTransactionIds.append(debtTransactionId);
        addDebtPayments(debtTransactionId,
                        newDebtTransactions.at(i).toMap().value("new_debt_payments").toList(),
                        newDebtPaymentIds);
    }
}

void AddDebtor::addDebtPayments(int debtTransactionId,
                                const QVariantList &newDebtPayments,
                                QVariantList &newDebtPaymentIds)
{
    for (const QVariant &debtPaymentAsVariant : newDebtPayments) {
        const QDateTime &dueDateTime = debtPaymentAsVariant.toMap().value("due_date_time").toDateTime();
        const double totalDebt = debtPaymentAsVariant.toMap().value("total_debt").toDouble();
        const double amountPaid = debtPaymentAsVariant.toMap().value("amount").toDouble();
        const double balance = debtPaymentAsVariant.toMap().value("balance").toDouble();
        const QString &note = debtPaymentAsVariant.toMap().value("note").toString();
        const int noteId = QueryExecutor::addNote(note,
                                                  QStringLiteral("debtor"),
                                                  ExceptionPolicy::DisallowExceptions);

        if (dueDateTime <= QDateTime::currentDateTime())
            throw InvalidDueDateException();

        const auto &records = callProcedure("AddDebtPayment", {
                                                ProcedureArgument {
                                                    ProcedureArgument::Type::In,
                                                    "debt_transaction_id",
                                                    debtTransactionId
                                                },
                                                ProcedureArgument {
                                                    ProcedureArgument::Type::In,
                                                    "total_debt",
                                                    totalDebt
                                                },
                                                ProcedureArgument {
                                                    ProcedureArgument::Type::In,
                                                    "amount_paid",
                                                    amountPaid
                                                },
                                                ProcedureArgument {
                                                    ProcedureArgument::Type::In,
                                                    "balance",
                                                    balance
                                                },
                                                ProcedureArgument {
                                                    ProcedureArgument::Type::In,
                                                    "currency",
                                                    Settings::DEFAULT_CURRENCY
                                                },
                                                ProcedureArgument {
                                                    ProcedureArgument::Type::In,
                                                    "due_date",
                                                    dueDateTime
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

        if (balance < 0.0)
            throw AmountOverpaidException(balance);

        newDebtPaymentIds.append(records.first().value("debt_payment_id").toInt());
    }
}
