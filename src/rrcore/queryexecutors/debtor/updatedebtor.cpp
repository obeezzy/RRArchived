#include "updatedebtor.h"
#include "database/databaseexception.h"
#include "user/userprofile.h"
#include "database/exceptions/exceptions.h"
#include "utility/debtorutils.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace DebtorQuery;

UpdateDebtor::UpdateDebtor(const Utility::Debtor &debtor,
                           QObject *receiver) :
    DebtorExecutor(COMMAND, {
                    { "can_undo", true },
                    { "debtor_id", debtor.id },
                    { "client_id", debtor.client.id },
                    { "preferred_name", debtor.client.preferredName },
                    { "first_name", debtor.client.firstName },
                    { "last_name", debtor.client.lastName },
                    { "image_url", debtor.client.imageUrl },
                    { "primary_phone_number", debtor.client.phoneNumber },
                    { "note_id", debtor.note.id },
                    { "note", debtor.note.note }
                   }, receiver)
{
    DebtorExecutor::arrangeDebtTransactions(debtor.transactions);
}

QueryResult UpdateDebtor::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    const QVariantMap &params = request().params();
    const int debtorId = params.value("debtor_id").toInt();
    const int clientId = params.value("client_id").toInt();
    const int noteId = params.value("note_id").toInt();
    const QString &note = params.value("note").toString();
    QVariantList newDebtTransactionIds;
    QVariantList updatedDebtTransactionIds;
    QVariantList archivedDebtTransactionIds;
    QVariantList newDebtTransactionNoteIds;
    QVariantList newDebtPaymentIds;
    QVariantList updatedDebtPaymentIds;
    QVariantList archivedDebtPaymentIds;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({ "client_id",
                                          "debtor_id",
                                          "preferred_name",
                                          "phone_number"
                                        }, params);

        QueryExecutor::beginTransaction(q);

        affirmDirtyTransactionsExist();
        QueryExecutor::addOrUpdateNote(noteId,
                                       note,
                                       QStringLiteral("debtor"),
                                       ExceptionPolicy::DisallowExceptions);
        addNotesForNewTransactions(newDebtTransactionNoteIds);
        updateNotesForExistingTransactions();
        updateClientDetails();
        addNewTransactions(newDebtTransactionNoteIds,
                           newDebtTransactionIds);
        updateExistingTransactions(updatedDebtTransactionIds);
        archiveDeletedTransactions(archivedDebtTransactionIds);
        addNewPayments(newDebtPaymentIds);
        updateExistingPayments(updatedDebtPaymentIds);
        archiveDeletedPayments(archivedDebtPaymentIds);

        result.setOutcome(QVariantMap {
                              { "client_id", clientId },
                              { "debtor_id", debtorId },
                              { "new_debt_transaction_ids", newDebtTransactionIds },
                              { "updated_debt_transaction_ids", updatedDebtTransactionIds },
                              { "archived_debt_transaction_ids", archivedDebtTransactionIds },
                              { "new_debt_payment_ids", newDebtPaymentIds },
                              { "updated_debt_payment_ids", updatedDebtPaymentIds },
                              { "archived_debt_payment_ids", archivedDebtPaymentIds }
                          });

        QueryExecutor::commitTransaction(q);
        return result;
    } catch (const DatabaseException &) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
}

void UpdateDebtor::affirmDirtyTransactionsExist()
{
    const QVariantMap &params = request().params();
    const QVariantList &newDebtTransactions = params.value("new_debt_transactions").toList();
    const QVariantList &updatedDebtTransactions = params.value("updated_debt_transactions").toList();
    const QVariantList &archivedDebtTransactions = params.value("archived_debt_transactions").toList();
    const QVariantList &archivedDebtPayments = params.value("archived_debt_payments").toList();

    if (updatedDebtTransactions.isEmpty()
            && newDebtTransactions.isEmpty()
            && archivedDebtTransactions.isEmpty()
            && archivedDebtPayments.isEmpty())
        throw MissingArgumentException(QStringLiteral("No new/updated/archived debt transactions for debtor %1.")
                                       .arg(params.value("preferred_name").toString()));
}

void UpdateDebtor::addNotesForNewTransactions(QVariantList &newDebtTransactionNoteIds)
{
    const QVariantMap &params = request().params();
    const QVariantList &newDebtTransactions = params.value("new_debt_transactions").toList();

    for (const QVariant &transactionAsVariant : newDebtTransactions) {
        const QString &note = transactionAsVariant.toMap().value("note").toString();
        newDebtTransactionNoteIds.append(QueryExecutor::addNote(note,
                                                                QStringLiteral("debtor"),
                                                                ExceptionPolicy::DisallowExceptions));
    }

    if (newDebtTransactions.count() != newDebtTransactionNoteIds.count())
        throw ArgumentMismatchException(QStringLiteral("Failed to match note ID count with transaction count."));
}

void UpdateDebtor::updateNotesForExistingTransactions()
{
    const QVariantMap &params = request().params();
    const QVariantList &updatedDebtTransactions = params.value("updated_debt_transactions").toList();

    for (const QVariant &transactionAsVariant : updatedDebtTransactions) {
        const int noteId = transactionAsVariant.toMap().value("note_id").toInt();
        const QString &note = transactionAsVariant.toMap().value("note").toString();
        QueryExecutor::addOrUpdateNote(noteId,
                                       note,
                                       QStringLiteral("debtor"),
                                       ExceptionPolicy::DisallowExceptions);
    }
}

void UpdateDebtor::updateClientDetails()
{
    const QVariantMap &params = request().params();
    const int clientId = params.value("client_id").toInt();
    const QByteArray &imageBlob = QueryExecutor::imageUrlToByteArray(params.value("image_url").toUrl());
    Q_UNUSED(imageBlob)
    if (clientId <= 0)
        throw InvalidArgumentException(QStringLiteral("Client ID invalid."));

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
                          params.value("phone_number")
                      },
                      ProcedureArgument {
                          ProcedureArgument::Type::In,
                          "user_id",
                          UserProfile::instance().userId()
                      }
                  });
}

void UpdateDebtor::addNewTransactions(const QVariantList &newDebtTransactionNoteIds,
                                      QVariantList &newDebtTransactionIds)
{
    const QVariantMap &params = request().params();
    const int debtorId = params.value("debtor_id").toInt();
    const QVariantList &newDebtTransactions = params.value("new_debt_transactions").toList();

    for (int i = 0; i < newDebtTransactions.count(); ++i) {
        const int debtTransactionId = newDebtTransactions.at(i).toMap().value("debt_transaction_id").toInt();

        if (debtorId <= 0)
            throw InvalidArgumentException(QStringLiteral("Invalid debtor ID."));
        if (debtTransactionId <= 0)
            throw InvalidArgumentException(QStringLiteral("Invalid debt transaction ID."));

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
                              debtTransactionId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "note_id",
                              newDebtTransactionNoteIds.at(i)
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        newDebtTransactionIds.append(debtTransactionId);
    }
}

void UpdateDebtor::updateExistingTransactions(QVariantList &updatedDebtTransactionIds)
{
    const QVariantMap &params = request().params();
    const QVariantList &updatedDebtTransactions = params.value("updated_debt_transactions").toList();

    for (const QVariant &transactionAsVariant : updatedDebtTransactions) {
        const int debtTransactionId = transactionAsVariant.toMap().value("debt_transaction_id").toInt();

        if (debtTransactionId <= 0)
            throw InvalidArgumentException(QStringLiteral("Invalid debt transaction ID."));

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

        updatedDebtTransactionIds.append(debtTransactionId);
    }
}

void UpdateDebtor::archiveDeletedTransactions(QVariantList &archivedDebtTransactionIds)
{
    const QVariantMap &params = request().params();
    const QVariantList &archivedDebtTransactions = params.value("archived_debt_transactions").toList();

    for (const QVariant &transactionAsVariant : archivedDebtTransactions) {
        const int debtTransactionId = transactionAsVariant.toMap().value("debt_transaction_id").toInt();

        if (debtTransactionId <= 0)
            throw InvalidArgumentException(QStringLiteral("Invalid debt transaction ID."));

        callProcedure("ArchiveDebtTransactionById", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "debt_transaction_id",
                              transactionAsVariant.toMap().value("debt_transaction_id")
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        archivedDebtTransactionIds.append(debtTransactionId);
    }
}

void UpdateDebtor::addNewPayments(QVariantList &newDebtPaymentIds)
{
    const QVariantMap &params = request().params();
    const QVariantList &newDebtPayments = params.value("new_debt_payments").toList();

    for (const QVariant &paymentAsVariant : newDebtPayments) {
        const int debtTransactionId = paymentAsVariant.toMap().value("debt_transaction_id").toInt();
        const double totalAmount = paymentAsVariant.toMap().value("total_amount").toDouble();
        const double amountPaid = paymentAsVariant.toMap().value("amount_paid").toDouble();
        const double balance = paymentAsVariant.toMap().value("balance").toDouble();
        const QDateTime &dueDateTime = paymentAsVariant.toMap().value("due_date").toDateTime();
        const QString &note = paymentAsVariant.toMap().value("note").toString();
        const QString &currency = Settings::DEFAULT_CURRENCY;
        const int noteId = QueryExecutor::addNote(note,
                                                  QStringLiteral("debtor"),
                                                  ExceptionPolicy::DisallowExceptions);

        if (debtTransactionId <= 0)
            throw InvalidArgumentException(QStringLiteral("Debt transaction ID for payment is invalid."));
        if (totalAmount < amountPaid)
            throw InvalidArgumentException(QStringLiteral("Amount paid cannot exceed total debt."));
        if (balance != (totalAmount - amountPaid))
            throw InvalidArgumentException(QStringLiteral("Invalid balance."));
        if (dueDateTime.isNull() || !dueDateTime.isValid() || dueDateTime <= QDateTime::currentDateTime())
            throw InvalidDueDateException();

        const QList<QSqlRecord> &records = callProcedure("AddDebtPayment", {
                                                             ProcedureArgument {
                                                                 ProcedureArgument::Type::In,
                                                                 "debt_transaction_id",
                                                                 debtTransactionId
                                                             },
                                                             ProcedureArgument {
                                                                 ProcedureArgument::Type::In,
                                                                 "total_amount",
                                                                 totalAmount
                                                             },
                                                             ProcedureArgument {
                                                                 ProcedureArgument::Type::In,
                                                                 "amount_paid",
                                                                 amountPaid
                                                             },
                                                             ProcedureArgument {
                                                                 ProcedureArgument::Type::In,
                                                                 "balance",
                                                                 qAbs(balance)
                                                             },
                                                             ProcedureArgument {
                                                                 ProcedureArgument::Type::In,
                                                                 "currency",
                                                                 currency
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
            throw AmountOverpaidException(totalAmount);
        if (records.isEmpty())
            throw UnexpectedResultException(QStringLiteral("Expected a debt payment ID but got nothing back."));

        const int debtPaymentId = records.first().value("debt_payment_id").toInt();
        newDebtPaymentIds.append(debtPaymentId);
    }
}

void UpdateDebtor::updateExistingPayments(QVariantList &updatedDebtPaymentIds)
{
    const QVariantMap &params = request().params();
    const QVariantList &updatedDebtPayments = params.value("updated_debt_payments").toList();

    for (const QVariant &paymentAsVariant : updatedDebtPayments) {
        const int debtPaymentId = paymentAsVariant.toMap().value("debt_payment_id").toInt();
        const double totalAmount = paymentAsVariant.toMap().value("total_amount").toDouble();
        const double amountPaid = paymentAsVariant.toMap().value("amount_paid").toDouble();
        const double balance = paymentAsVariant.toMap().value("balance").toDouble();
        const QDateTime &dueDateTime = paymentAsVariant.toMap().value("due_date").toDateTime();
        const QString &note = paymentAsVariant.toMap().value("note").toString();
        const QString &currency = Settings::DEFAULT_CURRENCY;
        const int noteId = QueryExecutor::addNote(note,
                                                  QStringLiteral("debtor"),
                                                  ExceptionPolicy::DisallowExceptions);

        if (debtPaymentId <= 0)
            throw InvalidArgumentException(QStringLiteral("Invalid debt payment ID."));
        if (dueDateTime.isNull() || !dueDateTime.isValid() || dueDateTime <= QDateTime::currentDateTime())
            throw InvalidDueDateException();
        if (balance < 0.0)
            throw AmountOverpaidException(totalAmount);

        callProcedure("UpdateDebtPayment", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "debt_payment_id",
                              debtPaymentId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "total_amount",
                              totalAmount
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "amount_paid",
                              amountPaid
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "balance",
                              qAbs(balance)
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "currency",
                              currency
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

        QueryExecutor::addOrUpdateNote(noteId,
                                       note,
                                       QStringLiteral("debtor"),
                                       ExceptionPolicy::DisallowExceptions);
        updatedDebtPaymentIds.append(debtPaymentId);
    }
}

void UpdateDebtor::archiveDeletedPayments(QVariantList &archivedDebtPaymentIds)
{
    const QVariantMap &params = request().params();
    const QVariantList &archivedDebtPayments = params.value("archived_debt_payments").toList();

    for (const QVariant &paymentAsVariant : archivedDebtPayments) {
        const int debtPaymentId = paymentAsVariant.toMap().value("debt_payment_id").toInt();

        if (debtPaymentId <= 0)
            throw InvalidArgumentException("Invalid debt payment ID.");

        callProcedure("ArchiveDebtPayment", {
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "debt_payment_id",
                              debtPaymentId
                          },
                          ProcedureArgument {
                              ProcedureArgument::Type::In,
                              "user_id",
                              UserProfile::instance().userId()
                          }
                      });

        archivedDebtPaymentIds.append(debtPaymentId);
    }
}
