#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QString>
#include <QVariantMap>
#include <QDateTime>
#include <QFlags>
#include <QUrl>

namespace Utility {
struct Note {
    int id;
    QString note;

    explicit Note() :
        id(-1) { }
    explicit Note(int id,
                  const QString &note) :
        id(id),
        note(note) { }
    explicit Note(const QString &note) :
        id(-1),
        note(note) { }
    explicit Note(const QVariantMap &map) :
        id(map.value("note_id").toInt()),
        note(map.value("note").toString()) { }

    QVariantMap toVariantMap() const {
        return {
            { "note_id", id },
            { "note", note }
        };
    }
};

struct Client {
    int id;
    QString preferredName;
    QString phoneNumber;
    QString firstName;
    QString lastName;
    QUrl imageUrl;
    QString emailAddress;

    explicit Client() :
        id(-1) { }
    explicit Client(const QString &preferredName) :
        id(-1),
        preferredName(preferredName) { }
    explicit Client(int id,
                    const QString &preferredName,
                    const QString &phoneNumber = QString()) :
        id(id),
        preferredName(preferredName),
        phoneNumber(phoneNumber) { }
    explicit Client(const QString &preferredName,
                    const QString &phoneNumber,
                    const QString &firstName,
                    const QString &lastName,
                    const QUrl &imageUrl = QUrl(),
                    const QString &emailAddress = QString()) :
        id(-1),
        preferredName(preferredName),
        phoneNumber(phoneNumber),
        firstName(firstName),
        lastName(lastName),
        imageUrl(imageUrl),
        emailAddress(emailAddress) { }
    explicit Client(int id,
                    const QString &preferredName,
                    const QString &phoneNumber,
                    const QString &firstName,
                    const QString &lastName,
                    const QUrl &imageUrl = QUrl(),
                    const QString &emailAddress = QString()) :
        id(id),
        preferredName(preferredName),
        phoneNumber(phoneNumber),
        firstName(firstName),
        lastName(lastName),
        imageUrl(imageUrl),
        emailAddress(emailAddress) { }
};

struct RelatedTransaction {
    int id;
    QString tableName;

    explicit RelatedTransaction() :
        id(-1) { }
    explicit RelatedTransaction(int id, const QString &tableName) :
        id(id), tableName(tableName) { }
};

struct Customer {
    int id;
    Client client;

    explicit Customer() :
        id(-1) { }
    explicit Customer(const Client &client) :
        id(-1),
        client(client) { }
    explicit Customer(int id,
                      const Client &client) :
        id(id),
        client(client) { }
};

struct Vendor {
    int id;
    Client client;

    explicit Vendor(const Client &client) :
        id(-1),
        client(client) { }

    explicit Vendor(int id,
                    const Client &client) :
        id(id),
        client(client) { }
};

struct FilterCriteria {
    QString column;
    QString text;

    explicit FilterCriteria(const QString &text) :
        text(text) { }

    explicit FilterCriteria(const QString &column,
                            const QString &text) :
        column(column),
        text(text) { }
};

struct SortCriteria {
    QString column;
    Qt::SortOrder order;

    explicit SortCriteria(Qt::SortOrder order) :
        order(order) { }

    explicit SortCriteria(const QString &column,
                          Qt::SortOrder order) :
        column(column),
        order(order) { }

    QString orderAsString() const {
        return order == Qt::AscendingOrder ? QStringLiteral("ascending")
                                           : QStringLiteral("descending");
    }
};

struct DateTimeSpan {
    QDateTime from;
    QDateTime to;

    explicit DateTimeSpan(const QDateTime &from,
                          const QDateTime &to) :
        from(from),
        to(to) { }
};

class RecordGroup
{
 public:
    enum Flag {
        None = 0x0,
        Archived = 0x1,
        Suspended = 0x2,
        Tracked = 0x4,
        Divisible = 0x8,
        Active = 0x16
    }; Q_DECLARE_FLAGS(Flags, Flag)
};

enum class PaymentMethod {
    Cash,
    DebitCard,
    CreditCard
};

inline static QString asString(const PaymentMethod &paymentMethod) {
    switch (paymentMethod) {
    case PaymentMethod::Cash:
        return QStringLiteral("cash");
    case PaymentMethod::DebitCard:
        return QStringLiteral("debit_card");
    case PaymentMethod::CreditCard:
        return QStringLiteral("credit_card");
    }

    return QString();
}
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Utility::RecordGroup::Flags)
#endif // COMMONUTILS_H
