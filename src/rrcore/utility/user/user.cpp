#include "user.h"
#include <QString>

using namespace Utility;

User::User(const QVariantMap &map)
{
    Q_UNUSED(map)
}

User::User(const QString &user,
           const QString &password) :
    user(user),
    password(password)
{}

User::User(int id,
           const QString &user) :
    id(id),
    user(user)
{}

User::User(int id,
           const QString &user,
           const QString &firstName,
           const QString &lastName,
           const QUrl &imageUrl,
           const QString &phoneNumber,
           const QString &emailAddress,
           const Note &note,
           const RecordGroup::Flags &flags) :
    id(id),
    user(user),
    firstName(firstName),
    lastName(lastName),
    imageUrl(imageUrl),
    phoneNumber(phoneNumber),
    emailAddress(emailAddress),
    note(note),
    flags(flags)
{}

User::User(const QString &firstName,
           const QString &lastName,
           const QString &user,
           const QString &password,
           const QString &phoneNumber,
           const QString &emailAddress,
           const QUrl &imageUrl) :
    user(user),
    password(password),
    firstName(firstName),
    lastName(lastName),
    imageUrl(imageUrl),
    phoneNumber(phoneNumber),
    emailAddress(emailAddress)
{}

QVariantMap User::toVariantMap() const
{
    return {};
}
