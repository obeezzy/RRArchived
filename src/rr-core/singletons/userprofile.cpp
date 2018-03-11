#include "userprofile.h"
#include <QCoreApplication>

UserProfile *UserProfile::m_instance = nullptr;

UserProfile::UserProfile(QObject *parent)
    : m_userId(0)
    , QObject(parent)
{

}

void UserProfile::setUser(int userId, const QString &userName)
{
    m_userId = userId;
    m_userName = userName;
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
