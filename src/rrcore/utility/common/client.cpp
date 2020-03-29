#include "client.h"
#include <QVariantList>
#include <QVariantMap>

using namespace Utility;

Client::Client(const QString &preferredName) :
    preferredName(preferredName)
{

}

Client::Client(int id,
               const QString &preferredName,
               const QString &phoneNumber) :
    id(id),
    preferredName(preferredName),
    phoneNumber(phoneNumber)
{

}

Client::Client(const QString &preferredName,
               const QString &phoneNumber,
               const QString &firstName,
               const QString &lastName,
               const QUrl &imageUrl,
               const QString &emailAddress) :
    preferredName(preferredName),
    phoneNumber(phoneNumber),
    firstName(firstName),
    lastName(lastName),
    imageUrl(imageUrl),
    emailAddress(emailAddress)
{

}

Client::Client(int id,
               const QString &preferredName,
               const QString &phoneNumber,
               const QString &firstName,
               const QString &lastName,
               const QUrl &imageUrl,
               const QString &emailAddress) :
    id(id),
    preferredName(preferredName),
    phoneNumber(phoneNumber),
    firstName(firstName),
    lastName(lastName),
    imageUrl(imageUrl),
    emailAddress(emailAddress)
{

}

Client::Client(const QVariantMap &map) :
    id(map.value("client_id").toInt()),
    preferredName(map.value("preferred_name").toString()),
    phoneNumber(map.value("phone_number").toString()),
    firstName(map.value("first_name").toString()),
    lastName(map.value("last_name").toString()),
    imageUrl(map.value("image_url").toUrl()),
    emailAddress(map.value("email_address").toString())
{}

QVariantMap Client::toVariantMap() const {
    return {
        { "client_id", id },
        { "preferred_name", preferredName },
        { "phone_number", phoneNumber },
        { "first_name", firstName },
        { "last_name", lastName },
        { "image_url", imageUrl },
        { "email_address", emailAddress }
    };
}
