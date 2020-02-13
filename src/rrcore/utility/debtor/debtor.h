#ifndef DEBTOR_H
#define DEBTOR_H

#include "utility/commonutils.h"
#include "debttransaction.h"
#include <QString>
#include <QUrl>

namespace Utility {
struct Debtor
{
    int id;
    Client client;
    DebtTransactionList debtTransactions;
    Note note;
    int row;

    explicit Debtor(int id);
    explicit Debtor(const Client &client,
                    const DebtTransactionList &debtTransactions,
                    const Note &note);

    explicit Debtor(int id,
                    const Client &client,
                    const DebtTransactionList &debtTransactions,
                    const Note &note);

    explicit Debtor(int id,
                    int row);
};
}

#endif // DEBTOR_H
