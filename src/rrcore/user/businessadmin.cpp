#include "businessadmin.h"
#include <QDebug>

BusinessAdmin::BusinessAdmin(QObject *parent) :
    QObject(parent)
{

}

void BusinessAdmin::extractFromVariantMap(const QVariantMap &map)
{
    if (map.isEmpty())
        return;

    m_emailAddress = map.value("email_address").toString();
    m_password = map.value("password").toString();
    m_businessStores.clear();
    QList<BusinessStore> stores;
    for (const QVariant &variant : map.value("business_stores").toList()) {
        const QVariantMap &map = variant.toMap();
        stores.append(BusinessStore::fromVariantMap(map));
    }

    setBusinessStores(stores);
}

void BusinessAdmin::setEmailAddress(const QString &emailAddress)
{
    m_emailAddress = emailAddress;
}

void BusinessAdmin::setPassword(const QString &password)
{
    m_password = password;
}

void BusinessAdmin::setBusinessStores(const QList<BusinessStore> &businessStores)
{
    if (m_businessStores == businessStores)
        return;

    m_businessStores = businessStores;
    emit businessStoresChanged();
}
