#include "logger.h"
#include <QLoggingCategory>
#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>

const QString LOG_FILE = QStringLiteral("/tmp/rr.log");

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)
    QByteArray localMsg = msg.toLocal8Bit();
    if (Logger::instance().isEnabled()) {
        switch (type) {
        case QtDebugMsg:
            //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            fprintf(stderr, "[DEBUG] %s: %s\n", context.category, localMsg.constData());
            break;
        case QtInfoMsg:
            fprintf(stderr, "[INFO] %s: %s\n", context.category, localMsg.constData());
            break;
        case QtWarningMsg:
            fprintf(stderr, "[WARNING] %s: %s\n", context.category, localMsg.constData());
            break;
        case QtCriticalMsg:
            fprintf(stderr, "[CRITICAL] %s: %s\n", context.category, localMsg.constData());
            break;
        case QtFatalMsg:
            fprintf(stderr, "[FATAL] %s: %s\n", context.category, localMsg.constData());
            abort();
        }
    }

    if (Logger::instance().isFileLoggingEnabled())
        Logger::instance().logToFile(localMsg.append('\n'));
}

Logger::Logger() :
    m_enabled(true)
{
    const QString &logFileName = m_settings.value("log_file_name", LOG_FILE).toString();
    m_logFile.setFileName(logFileName);
    m_logFile.open(QFile::WriteOnly);
    m_logFile.resize(0);
}

Logger &Logger::instance()
{
    static Logger instance;
    return instance;
}

Logger::~Logger()
{
    m_logFile.close();
}

void Logger::start()
{
    //QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));

    qInstallMessageHandler(messageHandler); // Install the handler
}

bool Logger::isEnabled() const
{
    return m_enabled;
}

void Logger::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool Logger::isFileLoggingEnabled() const
{
    return m_settings.value("log_to_file", true).toBool();
}

void Logger::setFileLoggingEnabled(bool enabled)
{
    m_settings.setValue("log_to_file", enabled);
}

void Logger::logToFile(const QByteArray &log)
{
    if (log.isEmpty())
        return;

    static QMutex mutex;
    QMutexLocker lock(&mutex);
    m_logFile.write(log);
}
