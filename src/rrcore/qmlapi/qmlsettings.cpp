#include "qmlsettings.h"
#include "singletons/settings.h"

QMLSettings::QMLSettings(QObject *parent) :
    QObject(parent)
{
    connect(&Settings::instance(), &Settings::darkModeActiveChanged, this, &QMLSettings::darkModeActiveChanged);
}

bool QMLSettings::darkModeActive() const
{
    return Settings::instance().darkModeActive();
}

void QMLSettings::setDarkModeActive(bool darkModeActive)
{
    Settings::instance().setDarkModeActive(darkModeActive);
}
