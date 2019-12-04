#include "viewdebttransactions.h"
#include "database/databaseexception.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

using namespace DebtorQuery;

ViewDebtTransactions::ViewDebtTransactions(int debtorId,
                                           QObject *receiver) :
    DebtorExecutor(COMMAND, { { "debtor_id", debtorId } }, receiver)
{

}

QueryResult ViewDebtTransactions::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request().params();
    int clientId = 0;
    QString preferredName;
    QString primaryPhoneNumber;
    QString debtorNote;
    QSqlQuery q(connection);
    QList<QSqlRecord> records;

    try {
        QueryExecutor::enforceArguments({ "debtor_id" }, params);

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
            const QDateTime &created = record.value("debt_transaction_created").toDateTime();

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
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
