#include "userprofile.h"
#include <QCoreApplication>
#include <QUrl>

UserProfile *UserProfile::m_instance = nullptr;

UserProfile::UserProfile(QObject *parent) :
    QObject(parent),
    m_userName(QString()),
    m_userId(0),
    m_businessDetails(new BusinessDetails(this))
{

}

void UserProfile::setUser(int userId, const QString &userName, const QVariant &privileges)
{
    m_userId = userId;
    m_userName = userName;
    m_privileges = privileges;
}

UserProfile &UserProfile::instance()
{
    if (m_instance == nullptr) {
        m_instance = new UserProfile;
        connect(qApp, &QCoreApplication::aboutToQuit, m_instance, &UserProfile::deleteLater);
    }

    return *m_instance;
}

int UserProfile::userId() const
{
    return m_userId;
}

QString UserProfile::userName() const
{
    return m_userName;
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
