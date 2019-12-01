#ifndef BUSINESSSTORE_H
#define BUSINESSSTORE_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QVariantMap>
#include <QDebug>

class QMLUserProfile;

class BusinessStore : public QObject {
    friend class QMLUserProfile;
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString address READ address CONSTANT)
    Q_PROPERTY(QString businessFamily READ businessFamily CONSTANT)
    Q_PROPERTY(int establishmentYear READ establishmentYear CONSTANT)
    Q_PROPERTY(QString phoneNumber READ phoneNumber CONSTANT)
    Q_PROPERTY(QUrl logoUrl READ logoUrl CONSTANT)
    Q_PROPERTY(QString rackId READ rackId CONSTANT)
public:
    explicit BusinessStore(QObject *parent = nullptr);
    BusinessStore(const BusinessStore &other);
    BusinessStore &operator= (const BusinessStore &other);

    bool operator==(const BusinessStore &other);

    int id() const;
    QString name() const;
    QString address() const;
    QString businessFamily() const;
    int establishmentYear() const;
    QString phoneNumber() const;
    QUrl logoUrl() const;
    QString rackId() const;

    static BusinessStore fromVariantMap(const QVariantMap &map);
    QVariantMap toVariantMap() const;

    friend QDebug operator<<(QDebug debug, const BusinessStore &businessStore)
    {
       debug.nospace() << "BusinessStore("
                            << "name=" << businessStore.name()
                            << ", address=" << businessStore.address()
                            << ", businessFamily=" << businessStore.businessFamily()
                            << ", establishmentYear=" << businessStore.establishmentYear()
                            << ", phoneNumber=" << businessStore.phoneNumber()
                            << ", logoUrl=" << businessStore.logoUrl()
                            << ", rackId=" << businessStore.rackId()
                            << ")";

        return debug;
    }
private:
    int m_id;
    QString m_name;
    QString m_address;
    QString m_businessFamily;
    int m_establishmentYear;
    QString m_phoneNumber;
    QUrl m_logoUrl;
    QString m_rackId;

    void setId(int id);
    void setName(const QString &name);
    void setAddress(const QString &address);
    void setBusinessFamily(const QString &businessFamily);
    void setEstablishmentYear(int establishmentYear);
    void setPhoneNumber(const QString &phoneNumber);
    void setLogoUrl(const QUrl &logoUrl);
    void setRackId(const QString &rackId);
};

#endif // BUSINESSSTORE_H
