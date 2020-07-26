#ifndef SETTINGS_H
#define SETTINGS_H

#include <QLocale>
#include <QObject>
#include <QSettings>
#include <QUrl>

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings& instance();

    Settings(Settings const&) = delete;
    void operator=(Settings const&) = delete;

    bool darkModeActive() const;
    void setDarkModeActive(bool darkModeActive);

    int cashPaymentLimit() const;
    int cardPaymentLimit() const;

    QLocale locale() const;

    static QUrl defaultReceiptTemplateUrl();
signals:
    void darkModeActiveChanged();

private:
    explicit Settings(QObject* parent = nullptr);
    QSettings m_settings;
    bool m_darkModeActive;
};

#endif  // SETTINGS_H
