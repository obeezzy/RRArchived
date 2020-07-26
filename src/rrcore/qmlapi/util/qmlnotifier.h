#ifndef QMLNOTIFIER_H
#define QMLNOTIFIER_H

#include <QObject>
#include <QUrl>
#include <QLoggingCategory>
#include "network/serverresponse.h"

class QQmlEngine;
class QJSEngine;

// Urgency (u) - Low, Normal, Critical
// Expire-time (t) - Timeout in ms
// App-name (a) - App name for the icon
// Icon (i) - icon file name
// Category (c) - Notification category

class QMLNotifier : public QObject
{
    Q_OBJECT
public:
    enum class Urgency {
        LowPriority,
        NormalPriority,
        HighPriority
    }; Q_ENUM(Urgency)

    enum class Category {
        Stock,
        Sales
    }; Q_ENUM(Category)

    enum class Duration {
        Short,
        Long
    }; Q_ENUM(Duration)

    explicit QMLNotifier(QObject *parent = nullptr);

    Q_INVOKABLE void show(Category category, const QString &title, const QString &message,
                          Urgency urgency = Urgency::NormalPriority, Duration duration = Duration::Short);
private:
    void displayNotificationOnWindows(QMLNotifier::Category category, const QString &title,
                                      const QString &message, QMLNotifier::Urgency urgency,
                                      QMLNotifier::Duration duration, const QUrl &iconUrl, const QString &appName);
    void displayNotificationOnLinux(Category category, const QString &title, const QString &message,
                                    Urgency urgency = Urgency::NormalPriority, Duration duration = Duration::Short,
                                    const QUrl &iconUrl = QUrl(), const QString &appName = QString());

    void displayServerStatus(const ServerResponse response);
};

static QObject *notifier_provider (QQmlEngine *, QJSEngine *) {
    QMLNotifier *notifier = new QMLNotifier();
    return notifier;
}

Q_DECLARE_LOGGING_CATEGORY(qmlNotifier);

#endif // QMLNOTIFIER_H
