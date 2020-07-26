#include "utility/debtor/debtor.h"

using namespace Utility::Debtor;

Debtor::Debtor(const QVariantMap &map) :
    id(map.value("debtor_id").toInt()),
    client(Client{ map }),
    totalDebt(map.value("total_debt").toDouble()),
    note(Note{ map }),
    timestamp(RecordTimestamp{ map }),
    user(User::User{ map }),
    row(map.value("row", -1).toInt())
{}

Debtor::Debtor(int id) :
    id(id)
{}

Debtor::Debtor(const Client &client,
               const DebtTransactionList &transactions,
               const Note &note) :
    client(client),
    transactions(transactions),
    note(note)
{}

Debtor::Debtor(int id,
               const Client &client,
               const DebtTransactionList &transactions,
               const Note &note) :
    id(id),
    client(client),
    transactions(transactions),
    note(note)
{}

QVariantMap Debtor::toVariantMap() const
{
    return {
        { "debtor_id", id },
        { "client_id", client.id },
        { "preferred_name", client.preferredName },
        { "phone_number", client.phoneNumber },
        { "first_name", client.firstName },
        { "last_name", client.lastName },
        { "image_url", client.imageUrl },
        { "email_address", client.emailAddress },
        { "note_id", note.id },
        { "note", note.note },
        { "created", timestamp.created },
        { "last_edited", timestamp.lastEdited },
        { "user_id", user.id },
        { "username", user.user },
        { "row", row }
    };
}
