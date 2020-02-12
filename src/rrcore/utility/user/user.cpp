#include "user.h"
#include <QString>

User::User() :
    id(-1),
    active(false),
    archived(false),
    userId(-1)
{

}

User::User(const QString &user,
           const QString &password) :
    id(-1),
    user(user),
    password(password),
    active(false),
    archived(false),
    userId(-1)
{

}

User::User(int id,
           const QString &user) :
    id(id),
    user(user),
    active(false),
    archived(false),
    userId(-1)
{

}

User::User(int id,
           const QString &user,
           const QString &firstName,
           const QString &lastName,
           const QUrl &imageUrl,
           const QString &phoneNumber,
           const QString &emailAddress,
           bool active,
           const Note &note,
           bool archived) :
    id(id),
    user(user),
    firstName(firstName),
    lastName(lastName),
    imageUrl(imageUrl),
    phoneNumber(phoneNumber),
    emailAddress(emailAddress),
    active(active),
    note(note),
    archived(archived),
    userId(-1)
{

}

User::User(const QString &firstName,
           const QString &lastName,
           const QString &user,
           const QString &password,
           const QString &phoneNumber,
           const QString &emailAddress,
           const QUrl &imageUrl) :
    id(-1),
    user(user),
    password(password),
    firstName(firstName),
    lastName(lastName),
    imageUrl(imageUrl),
    phoneNumber(phoneNumber),
    emailAddress(emailAddress),
    active(false),
    archived(false),
    userId(-1)
{

}
