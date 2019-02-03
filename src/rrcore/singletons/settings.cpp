#include "settings.h"
#include <QFileInfo>
#include <QDebug>

Settings::Settings(QObject *parent) :
    QObject(parent),
    m_darkModeActive(m_settings.value("appearance/theme").toString() == "dark")
{

}

Settings &Settings::instance()
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

QUrl Settings::defaultReceiptTemplateUrl()
{
    return QUrl("qrc:/settings/receipt/ReceiptTypeA.qml");
}
