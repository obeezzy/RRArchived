#include "config.h"

const QString CONFIG_FILE(":/config/config.ini");

Config::Config() :
    m_settings(CONFIG_FILE, QSettings::IniFormat)
{

}

Config &Config::instance()
{
    static Config instance;
    return instance;
}

QString Config::hostName()
{
    m_settings.beginGroup("Database");
    const QString &hostName = m_settings.value("host").toString();
    m_settings.endGroup();

    return hostName;
}

int Config::port()
{
    m_settings.beginGroup("Database");
    const int port = m_settings.value("port").toInt();
    m_settings.endGroup();

    return port;
}

QString Config::userName()
{
    m_settings.beginGroup("Database");
    const QString &userName = m_settings.value("user").toString();
    m_settings.endGroup();

    return userName;
}

QString Config::password()
{
    m_settings.beginGroup("Database");
    const QString &password = m_settings.value("password").toString();
    m_settings.endGroup();

    return password;
}

QString Config::databaseName()
{
    m_settings.beginGroup("Database");
    const QString &databaseName = m_settings.value("database").toString();
    m_settings.endGroup();

    return databaseName;
}
