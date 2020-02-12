#ifndef USER_H
#define USER_H

#include "utility/commonutils.h"
#include <QString>
#include <QUrl>
#include <QDateTime>

struct User {
    int id;
    QString user;
    QString password;
    QString firstName;
    QString lastName;
    QUrl imageUrl;
    QString phoneNumber;
    QString emailAddress;
    bool active;
    Note note;
    bool archived;
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
                  bool active,
                  const Note &note,
                  bool archived);
    explicit User(const QString &firstName,
                  const QString &lastName,
                  const QString &user,
                  const QString &password,
                  const QString &phoneNumber,
                  const QString &emailAddress,
                  const QUrl &imageUrl);
};

#endif // USER_H
