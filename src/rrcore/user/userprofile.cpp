#include "userprofile.h"
#include "businessdetails.h"
#include "businessadmin.h"
#include "utility/user/user.h"
#include <QCoreApplication>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>

UserProfile::UserProfile(QObject *parent) :
    QObject(parent),
    m_businessDetails(new BusinessDetails(this)),
    m_businessAdmin(new BusinessAdmin(this))
{
    setAccessToken(QByteArray());
}

void UserProfile::setUser(const Utility::User &user)
{
    m_user = user;
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
    m_user.id = userId;
}

void UserProfile::setAccessToken(const QByteArray &accessToken)
{
    QSettings().setValue("access_token", accessToken);
    m_user.accessToken = accessToken;
}

void UserProfile::setServerTunnelingEnabled(bool enabled)
{
    QSettings().setValue("server_tunneling_enabled", enabled);
}

void UserProfile::clearUser()
{
    UserProfile::instance().setUser(Utility::User());
}

UserProfile &UserProfile::instance()
{
    static UserProfile userProfile;
    return userProfile;
}

int UserProfile::userId() const
{
    return m_user.id;
}

QString UserProfile::userName() const
{
    return m_user.user;
}

QString UserProfile::password() const
{
    return m_user.password;
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
        { "user_name", m_user.user },
        { "user_id", m_user.id },
        { "password", m_user.password },
        { "rack_id", rackId() }
    };

    return QJsonDocument(jsonObject).toJson();
}
