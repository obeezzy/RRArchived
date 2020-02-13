#ifndef USER_H
#define USER_H

#include "utility/commonutils.h"
#include <QString>
#include <QUrl>
#include <QDateTime>

namespace Utility {
struct User {
    int id;
    QString user;
    QString password;
    QString firstName;
    QString lastName;
    QUrl imageUrl;
    QString phoneNumber;
    QString emailAddress;
    Note note;
    RecordGroup::Flags flags; // Active, Archived
    QDateTime created;
    QDateTime lastEdited;
    int userId;

    explicit User();
    explicit User(const QString &user,
                  const QString &password = QString());
    explicit User(int id,
                  const QString &user = QString());
    explicit User(int id,
                  const QString &user,
                  const QString &firstName,
                  const QString &lastName,
                  const QUrl &imageUrl,
                  const QString &phoneNumber,
                  const QString &emailAddress,
                  const Note &note,
                  const RecordGroup::Flags &flags);
    explicit User(const QString &firstName,
                  const QString &lastName,
                  const QString &user,
                  const QString &password,
                  const QString &phoneNumber,
                  const QString &emailAddress,
                  const QUrl &imageUrl);
};
}

#endif // USER_H
