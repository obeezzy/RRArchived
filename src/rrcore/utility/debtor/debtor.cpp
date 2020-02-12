#include "utility/debtor/debtor.h"

Debtor::Debtor(int id) :
    id(id),
    row(-1)
{

}

Debtor::Debtor(const Client &client,
               const DebtTransactionList &debtTransactions,
               const Note &note) :
    id(-1),
    client(client),
    debtTransactions(debtTransactions),
    note(note),
    row(-1)
{

}

Debtor::Debtor(int id,
               const Client &client,
               const DebtTransactionList &debtTransactions,
               const Note &note) :
    id(id),
    client(client),
    debtTransactions(debtTransactions),
    note(note),
    row(-1)
{

}

Debtor::Debtor(int id,
               int row) :
    id(id),
    row(row)
{

}
