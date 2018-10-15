#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>

class Config
{
public:
    static Config &instance();

    ~Config() = default;
    Config(Config const &) = delete;
    void operator=(Config const &) = delete;

    QString hostName();
    int port();
    QString userName();
    QString password();
    QString databaseName();
private:
    QSettings m_settings;
    Config();
};

#endif // CONFIG_H
