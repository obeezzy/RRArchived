#ifndef USER_H
#define USER_H

#include "utility/commonutils.h"
#include "userprivilege.h"
#include <QString>
#include <QUrl>
#include <QDateTime>

namespace Utility {
struct User {
    int id {-1};
    QString user;
    QString password;
    QString firstName;
    QString lastName;
    QUrl imageUrl;
    QString phoneNumber;
    QString emailAddress;
    QString preset;
    UserPrivilegeList privileges;
    Note note;
    RecordGroup::Flags flags; // Active, Archived
    QDateTime created;
    QDateTime lastEdited;
    QByteArray accessToken;
    int userId {-1};
    int row {-1};

    explicit User() = default;
    explicit User(const QVariantMap &map);
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

    QVariantMap toVariantMap() const;
};

class UserList : public QList<User>
{
public:
    explicit UserList() = default;
    UserList(std::initializer_list<User> users) :
        QList<User>(users) { }
    explicit UserList(const QVariantList &list) :
        QList<User>() {
        for (const auto &variant : list)
            append(User{ variant.toMap() });
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::User, Q_PRIMITIVE_TYPE);

#endif // USER_H
