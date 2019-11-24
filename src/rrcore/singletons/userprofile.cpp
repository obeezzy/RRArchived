#include "userprofile.h"
#include <QCoreApplication>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>

UserProfile::UserProfile(QObject *parent) :
    QObject(parent),
    m_userId(0),
    m_businessDetails(new BusinessDetails(this)),
    m_rackId("rr_server")
{
    QSettings().setValue("access_token", "");
}

void UserProfile::setUser(int userId,
                          const QString &userName,
                          const QString &password,
                          const QVariant &privileges,
                          const QByteArray &accessToken)
{
    m_userId = userId;
    m_userName = userName;
    m_password = password;
    m_privileges = privileges;
    QSettings().setValue("access_token", accessToken);
}

UserProfile &UserProfile::instance()
{
    static UserProfile userProfile;
    return userProfile;
}

int UserProfile::userId() const
{
    return m_userId;
}

QString UserProfile::userName() const
{
    return m_userName;
}

QString UserProfile::password() const
{
    return m_password;
}

bool UserProfile::isAdmin() const
{
    return false;
}

bool UserProfile::hasPrivilege(const QString &privilege) const
{
    if (isAdmin())
        return true;
    if (privilege.trimmed().isEmpty())
        return false;

    return true;
}

BusinessDetails *UserProfile::businessDetails() const
{
    return m_businessDetails;
}

QString UserProfile::rackId() const
{
    return m_rackId;
}

QByteArray UserProfile::accessToken() const
{
    return QSettings().value("access_token").toByteArray();
}

QByteArray UserProfile::toJson() const
{
    QJsonObject jsonObject {
        { "userName", m_userName },
        { "userId", m_userId },
        { "password", m_password },
        { "rackId", m_rackId }
    };

    return QJsonDocument(jsonObject).toJson();
}

BusinessDetails::BusinessDetails(QObject *parent) :
    QObject(parent),
    m_logoUrl(QUrl(QStringLiteral("qrc:/logo.png"))),
    m_name(QStringLiteral("Emeka and Jane")),
    m_address(QStringLiteral("1234 My Address, Apt #12, Ogun State, Nigeria")),
    m_phoneNumber(QStringLiteral("998877665544")),
    m_establishmentYear(1984)
{

}

QUrl BusinessDetails::logoUrl() const
{
    return m_logoUrl;
}

QString BusinessDetails::name() const
{
    return m_name;
}

void BusinessDetails::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged();
}

QString BusinessDetails::address() const
{
    return m_address;
}

void BusinessDetails::setAddress(const QString &address)
{
    if (m_address == address)
        return;

    m_address = address;
    emit addressChanged();
}

QString BusinessDetails::phoneNumber() const
{
    return m_phoneNumber;
}

void BusinessDetails::setPhoneNumber(const QString &phoneNumber)
{
    if (m_phoneNumber == phoneNumber)
        return;

    m_phoneNumber = phoneNumber;
    emit phoneNumberChanged();
}

int BusinessDetails::establishmentYear() const
{
    return m_establishmentYear;
}

void BusinessDetails::setLogoUrl(const QUrl &logoUrl)
{
    if (m_logoUrl == logoUrl)
        return;

    m_logoUrl = logoUrl;
    emit logoUrlChanged();
}

void BusinessDetails::setEstablishmentYear(int establishmentYear)
{
    if (m_establishmentYear == establishmentYear)
        return;

    m_establishmentYear = establishmentYear;
    emit establishmentYearChanged();
}
