#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QUrl>
#include <QVariantList>

namespace Utility {
struct Client {
    int id {-1};
    QString preferredName;
    QString phoneNumber;
    QString firstName;
    QString lastName;
    QUrl imageUrl;
    QString emailAddress;

    explicit Client() = default;
    explicit Client(const QVariantMap &map);
    explicit Client(const QString &preferredName);
    explicit Client(int id,
                    const QString &preferredName,
                    const QString &phoneNumber = QString());
    explicit Client(const QString &preferredName,
                    const QString &phoneNumber,
                    const QString &firstName,
                    const QString &lastName,
                    const QUrl &imageUrl = QUrl(),
                    const QString &emailAddress = QString());
    explicit Client(int id,
                    const QString &preferredName,
                    const QString &phoneNumber,
                    const QString &firstName,
                    const QString &lastName,
                    const QUrl &imageUrl = QUrl(),
                    const QString &emailAddress = QString());

    QVariantMap toVariantMap() const;
};

class ClientList : public QList<Client>
{
public:
    explicit ClientList() = default;
    ClientList(std::initializer_list<Client> clients) :
        QList<Client>(clients) { }
    explicit ClientList(const QVariantList &list) :
        QList<Client>() {
        for (const auto &variant : list)
            append(Client{ variant.toMap() });
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &client : *this)
            list.append(client.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::Client, Q_PRIMITIVE_TYPE);

#endif // CLIENT_H
