#ifndef LOGGER_H
#define LOGGER_H

#include <QSettings>
#include <QFile>

class Logger
{
public:
    static Logger &instance();

    Logger(Logger const &) = delete;
    void operator=(Logger const &) = delete;
    ~Logger();

    void start();

    bool isEnabled() const;
    void setEnabled(bool enabled);

    bool isFileLoggingEnabled() const;
    void setFileLoggingEnabled(bool enabled);

    void logToFile(const QByteArray &log);
private:
    QSettings m_settings;
    QFile m_logFile;
    bool m_enabled;
    bool m_fileLoggingEnabled;

    explicit Logger();
};

#endif // LOGGER_H
