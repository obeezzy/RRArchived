#include "settings.h"
#include <QDebug>
#include <QFileInfo>

const int CASH_PAYMENT_LIMIT = 1;
const int CARD_PAYMENT_LIMIT = 2;

Settings::Settings(QObject* parent)
    : QObject(parent),
      m_darkModeActive(m_settings.value("appearance/theme").toString() ==
                       "dark")
{
    QLocale::setDefault(QLocale(QLocale::English, QLocale::Nigeria));
}

Settings& Settings::instance()
{
    static Settings instance;
    return instance;
}

bool Settings::darkModeActive() const
{
    return m_darkModeActive;
}

void Settings::setDarkModeActive(bool darkModeActive)
{
    if (m_darkModeActive == darkModeActive)
        return;

    m_darkModeActive = darkModeActive;
    m_settings.setValue("appearance/theme", darkModeActive ? "dark" : "light");

    emit darkModeActiveChanged();
}

int Settings::cashPaymentLimit() const
{
    return CASH_PAYMENT_LIMIT;
}

int Settings::cardPaymentLimit() const
{
    return CARD_PAYMENT_LIMIT;
}

QLocale Settings::locale() const
{
    return QLocale();
}

QUrl Settings::defaultReceiptTemplateUrl()
{
    return QUrl("qrc:/settings/receipt/ReceiptTypeA.qml");
}
