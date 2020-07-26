#include "debtorexecutor.h"
#include "utility/debtor/debttransaction.h"
#include "utility/debtor/debtpayment.h"

DebtorExecutor::DebtorExecutor(const QString &command,
                               const QVariantMap &params,
                               QObject *receiver) :
    QueryExecutor(command, params, QueryRequest::QueryGroup::Debtor, receiver)
{

}

void DebtorExecutor::arrangeDebtTransactions(const Utility::Debtor::DebtTransactionList &debtTransactions)
{
    QVariantList newDebtTransactions;
    QVariantList updatedDebtTransactions;
    QVariantList archivedDebtTransactions;
    QVariantList newDebtPayments;
    QVariantList updatedDebtPayments;
    QVariantList archivedDebtPayments;

    for (const auto &transaction : debtTransactions) {
        if (transaction.state == Utility::Debtor::DebtTransaction::State::Fresh)
            newDebtTransactions.append(transaction.toVariantMap());
        else if (transaction.state == Utility::Debtor::DebtTransaction::State::Dirty)
            updatedDebtTransactions.append(transaction.toVariantMap());
        else if (transaction.state == Utility::Debtor::DebtTransaction::State::Trash)
            archivedDebtTransactions.append(transaction.toVariantMap());

        for (const auto &payment : transaction.payments) {
            if (payment.state == Utility::Debtor::DebtPayment::State::Fresh)
                newDebtPayments.append(payment.toVariantMap());
            else if (payment.state == Utility::Debtor::DebtPayment::State::Dirty)
                updatedDebtPayments.append(payment.toVariantMap());
            else if (payment.state == Utility::Debtor::DebtPayment::State::Trash)
                archivedDebtPayments.append(payment.toVariantMap());
        }

        for (const auto &payment : transaction.archivedPayments)
            archivedDebtPayments.append(payment.toVariantMap());
    }

    if (!newDebtTransactions.isEmpty())
        request().params().insert("new_debt_transactions", newDebtTransactions);
    if (!updatedDebtTransactions.isEmpty())
        request().params().insert("updated_debt_transactions", updatedDebtTransactions);
    if (!archivedDebtTransactions.isEmpty())
        request().params().insert("archived_debt_transactions", archivedDebtTransactions);
    if (!newDebtPayments.isEmpty())
        request().params().insert("new_debt_payments", newDebtPayments);
    if (!updatedDebtPayments.isEmpty())
        request().params().insert("updated_debt_payments", updatedDebtPayments);
    if (!archivedDebtPayments.isEmpty())
        request().params().insert("archived_debt_payments", archivedDebtPayments);
}
