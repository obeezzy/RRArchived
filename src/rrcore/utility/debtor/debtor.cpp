#include "utility/debtor/debtor.h"

using namespace Utility;

Debtor::Debtor(const QVariantMap &map) :
    id(map.value("debtor_id").toInt()),
    client(Client {
           map.value("preferred_name").toString(),
           map.value("phone_number").toString(),
           map.value("first_name").toString(),
           map.value("last_name").toString(),
           map.value("image_url").toUrl(),
           map.value("email_address").toString()
           }),
    created(map.value("created").toDateTime()),
    lastEdited(map.value("last_edited").toDateTime()),
    user(User {
         map.value("user_id").toInt(),
         map.value("user").toString()
         })
{ }

Debtor::Debtor(int id) :
    id(id)
{ }

Debtor::Debtor(const Client &client,
               const DebtTransactionList &transactions,
               const Note &note) :
    client(client),
    transactions(transactions),
    note(note)
{ }

Debtor::Debtor(int id,
               const Client &client,
               const DebtTransactionList &transactions,
               const Note &note) :
    id(id),
    client(client),
    transactions(transactions),
    note(note)
{ }

Debtor::Debtor(int id,
               int row) :
    id(id),
    row(row)
{ }

QVariantMap Debtor::toVariantMap() const
{
    return {
        { "debtor_id", id },
        { "preferred_name", client.preferredName },
        { "phone_number", client.phoneNumber },
        { "first_name", client.firstName },
        { "last_name", client.lastName },
        { "image_url", client.imageUrl },
        { "email_address", client.emailAddress },
        { "note_id", note.id },
        { "note", note.note },
        { "created", created },
        { "last_edited", lastEdited },
        { "user_id", user.id },
        { "user", user.user }
    };
}
