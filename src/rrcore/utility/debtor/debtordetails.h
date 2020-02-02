#ifndef DEBTORDETAILS_H
#define DEBTORDETAILS_H

#include "utility/commonutils.h"
#include <QString>
#include <QUrl>

struct DebtorDetails
{
    int debtorId;
    int clientId;
    QString preferredName;
    QString firstName;
    QString lastName;
    QString phoneNumber;
    QUrl imageUrl;
    Note note;

    explicit DebtorDetails(const QString &preferredName,
                           const QString &firstName,
                           const QString &lastName,
                           const QString &phoneNumber,
                           const QUrl &imageUrl,
                           const Note &note) :
        debtorId(-1),
        clientId(-1),
        preferredName(preferredName),
        firstName(firstName),
        lastName(lastName),
        phoneNumber(phoneNumber),
        imageUrl(imageUrl),
        note(note) { }

    explicit DebtorDetails(int debtorId,
                           int clientId,
                           const QString &preferredName,
                           const QString &firstName,
                           const QString &lastName,
                           const QString &phoneNumber,
                           const QUrl &imageUrl,
                           const Note &note) :
        debtorId(debtorId),
        clientId(clientId),
        preferredName(preferredName),
        firstName(firstName),
        lastName(lastName),
        phoneNumber(phoneNumber),
        imageUrl(imageUrl),
        note(note) { }
};

#endif // DEBTORDETAILS_H
