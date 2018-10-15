#include "config.h"

const QString configFile(":/config/config.ini");

Config::Config() :
    m_settings(configFile, QSettings::IniFormat)
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
    const QString &hostName = m_settings.value("hostName").toString();
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
    const QString &userName = m_settings.value("userName").toString();
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
    const QString &databaseName = m_settings.value("databaseName").toString();
    m_settings.endGroup();

    return databaseName;
}
