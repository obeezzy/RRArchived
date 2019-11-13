#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <QObject>
#include <QVariant>
#include <QUrl>
#include "qmlapi/qmluserprofile.h"

class BusinessDetails : public QObject {
    friend class QMLUserProfile;
    Q_OBJECT
    Q_PROPERTY(QUrl logoUrl READ logoUrl NOTIFY logoUrlChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString address READ address NOTIFY addressChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(int establishmentYear READ establishmentYear NOTIFY establishmentYearChanged)
public:
    explicit BusinessDetails(QObject *parent = nullptr);

    QUrl logoUrl() const;
    QString name() const;
    QString address() const;
    QString phoneNumber() const;
    int establishmentYear() const;
signals:
    void logoUrlChanged();
    void nameChanged();
    void addressChanged();
    void phoneNumberChanged();
    void establishmentYearChanged();
private:
    QUrl m_logoUrl;
    QString m_name;
    QString m_address;
    QString m_phoneNumber;
    int m_establishmentYear;

    void setLogoUrl(const QUrl &logoUrl);
    void setName(const QString &name);
    void setAddress(const QString &address);
    void setPhoneNumber(const QString &phoneNumber);
    void setEstablishmentYear(int establishmentYear);
};

class UserProfile : public QObject
{
    friend class QMLUserProfile;
    Q_OBJECT
public:
    static UserProfile &instance();

    int userId() const;
    QString userName() const;

    bool isAdmin() const;
    bool hasPrivilege(const QString &privilege) const;

    BusinessDetails *businessDetails() const;

    QString rackId() const;
signals:
    void adminChanged();
private:
    QUrl m_logoUrl;
    QString m_userName;
    int m_userId;
    QVariant m_privileges;
    BusinessDetails *m_businessDetails;
    QString m_rackId;

    explicit UserProfile(QObject *parent = nullptr);
    explicit UserProfile(const UserProfile &other) = delete;
    void setUser(int userId, const QString &userName, const QVariant &privileges);
};

#endif // USERPROFILE_H
