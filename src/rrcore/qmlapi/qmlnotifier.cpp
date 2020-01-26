#include "qmlnotifier.h"
#include <QVariant>
#include <QProcess>
#include <QLoggingCategory>
#include "network/networkthread.h"

// Urgency (u) - Low, Normal, Critical
// Expire-time (t) - Timeout in ms
// App-name (a) - App name for the icon
// Icon (i) - icon file name
// Category (c) - Notification category

Q_LOGGING_CATEGORY(qmlNotifier, "rrcore.qmlapi.qmlNotifier", QtWarningMsg);

const int SHORT_DURATION = 5000;
const int LONG_DURATION = 8000;
const QString LOGO_FILE(":/images/rr_logo.png");

QMLNotifier::QMLNotifier(QObject *parent) :
    QObject(parent)
{
    connect(&NetworkThread::instance(), &NetworkThread::responseReady,
            this, &QMLNotifier::displayServerStatus);
}

void QMLNotifier::show(QMLNotifier::Category category, const QString &title, const QString &message,
                       QMLNotifier::Urgency urgency, QMLNotifier::Duration duration)
{
#if defined(Q_OS_LINUX)
    displayNotificationOnLinux(category, title, message, urgency, duration);
#elif defined(Q_OS_WIN)
    displayNotificationOnWindows(category, title, message, urgency, duration);
#else
    qCritical() << "Record Rack does not support notifications for this platform.";
#endif
}

void QMLNotifier::displayNotificationOnLinux(QMLNotifier::Category category, const QString &title,
                                             const QString &message, QMLNotifier::Urgency urgency,
                                             QMLNotifier::Duration duration, const QUrl &iconUrl, const QString &appName)
{
    QProcess notifySend;
    QStringList arguments;
    switch (category) {
    case Category::Stock:
        arguments.append({ "-c", "stock" });
        break;
    case Category::Sales:
        arguments.append({ "-c", "sales" });
        break;
    }

    switch (urgency) {
    case Urgency::LowPriority:
        arguments.append({ "-u", "low" });
        break;
    case Urgency::NormalPriority:
        arguments.append({ "-u", "normal" });
        break;
    case Urgency::HighPriority:
        arguments.append({ "-u", "critical" });
        break;
    }

    switch (duration) {
    case Duration::Short:
        arguments.append({ "-t", QString::number(SHORT_DURATION) });
        break;
    case Duration::Long:
        arguments.append({ "-t", QString::number(LONG_DURATION) });
        break;
    }

    if (iconUrl.isEmpty()) {
        arguments.append({ "-i", LOGO_FILE });
    }

    if (appName.trimmed().isEmpty()) {
        arguments.append({ "-a", "Record Rack" });
    }

    arguments.append(title);
    arguments.append(message);

    notifySend.setProgram("notify-send");
    notifySend.setArguments(arguments);
    notifySend.start();
    notifySend.waitForStarted();
    notifySend.waitForFinished();
}

void QMLNotifier::displayServerStatus(const ServerResponse response)
{
    if (response.isSuccessful())
        show(Category::Stock, "There was no error", "No error at all!");
    else
        show(Category::Stock, "There was an error", "What happened?");
}

void QMLNotifier::displayNotificationOnWindows(QMLNotifier::Category category, const QString &title,
                                               const QString &message, QMLNotifier::Urgency urgency,
                                               QMLNotifier::Duration duration, const QUrl &iconUrl, const QString &appName)
{
    Q_UNUSED(category)
    Q_UNUSED(title)
    Q_UNUSED(message)
    Q_UNUSED(urgency)
    Q_UNUSED(duration)
    Q_UNUSED(iconUrl)
    Q_UNUSED(appName)
}
