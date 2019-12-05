#include "userprofile.h"
#include <QCoreApplication>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>

#include "database/databaseutils.h"
#include "businessdetails.h"
#include "businessadmin.h"

UserProfile::UserProfile(QObject *parent) :
    QObject(parent),
    m_userId(0),
    m_businessDetails(new BusinessDetails(this)),
    m_businessAdmin(new BusinessAdmin(this))
{
    setAccessToken(QByteArray());
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
    setAccessToken(accessToken);
}

void UserProfile::setDatabaseReady(bool databaseReady)
{
    QSettings().setValue("database_ready", databaseReady);
}

void UserProfile::setRackId(const QString &rackId)
{
    QSettings().setValue("rack_id", rackId);
}

void UserProfile::setUserId(int userId)
{
    m_userId = userId;
}

void UserProfile::setAccessToken(const QByteArray &accessToken)
{
    QSettings().setValue("access_token", accessToken);
}

void UserProfile::setServerTunnelingEnabled(bool enabled)
{
    QSettings().setValue("server_tunneling_enabled", enabled);
}

void UserProfile::clearUser()
{
    UserProfile::instance().setUser(-1, QString(), QString(), QVariant(), QByteArray());
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

bool UserProfile::isDatabaseReady() const
{
    return QSettings().value("database_ready", false).toBool();
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

bool UserProfile::isServerTunnelingEnabled() const
{
    return QSettings().value("server_tunneling_enabled", false).toBool();
}

BusinessDetails *UserProfile::businessDetails() const
{
    return m_businessDetails;
}

BusinessAdmin *UserProfile::businessAdmin() const
{
    return m_businessAdmin;
}

QString UserProfile::rackId() const
{
    return QSettings().value("rack_id", "NO_RACK_ID").toString();
}

QByteArray UserProfile::accessToken() const
{
    return QSettings().value("access_token").toByteArray();
}

QByteArray UserProfile::toJson() const
{
    QJsonObject jsonObject {
        { "user_name", m_userName },
        { "user_id", m_userId },
        { "password", m_password },
        { "rack_id", rackId() }
    };

    return QJsonDocument(jsonObject).toJson();
}
