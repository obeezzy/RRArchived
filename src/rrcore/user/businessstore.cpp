#include "businessstore.h"

#include "database/databaseutils.h"

BusinessStore::BusinessStore(QObject *parent) :
    QObject(parent),
    m_id(0),
    m_establishmentYear(0)
{

}

BusinessStore::BusinessStore(const BusinessStore &other) :
    QObject(nullptr)
{
    setId(other.id());
    setName(other.name());
    setAddress(other.address());
    setBusinessFamily(other.businessFamily());
    setEstablishmentYear(other.establishmentYear());
    setPhoneNumber(other.phoneNumber());
    setLogoUrl(other.logoUrl());
    setRackId(other.rackId());
}

BusinessStore &BusinessStore::operator=(const BusinessStore &other)
{
    m_id = other.id();
    m_name = other.name();
    m_address = other.address();
    m_businessFamily = other.businessFamily();
    m_establishmentYear = other.establishmentYear();
    m_phoneNumber = other.phoneNumber();
    m_logoUrl = other.logoUrl();
    m_rackId = other.rackId();

    return *this;
}

bool BusinessStore::operator==(const BusinessStore &other)
{
    return m_id == other.id()
            && m_name == other.name()
            && m_address == other.address()
            && m_businessFamily == other.businessFamily()
            && m_establishmentYear == other.establishmentYear()
            && m_phoneNumber == other.phoneNumber()
            && m_logoUrl == other.logoUrl()
            && m_rackId == other.rackId();
}

int BusinessStore::id() const
{
    return m_id;
}

QString BusinessStore::name() const
{
    return m_name;
}

QString BusinessStore::address() const
{
    return m_address;
}

QString BusinessStore::businessFamily() const
{
    return m_businessFamily;
}

int BusinessStore::establishmentYear() const
{
    return m_establishmentYear;
}

QString BusinessStore::phoneNumber() const
{
    return m_phoneNumber;
}

QUrl BusinessStore::logoUrl() const
{
    return m_logoUrl;
}

QString BusinessStore::rackId() const
{
    return m_rackId;
}

BusinessStore BusinessStore::fromVariantMap(const QVariantMap &map)
{
    BusinessStore businessStore;
    businessStore.setName(map.value("name").toString());
    businessStore.setAddress(map.value("address").toString());
    businessStore.setBusinessFamily(map.value("business_family").toString());
    businessStore.setEstablishmentYear(map.value("establishment_year").toInt());
    businessStore.setPhoneNumber(map.value("phone_number").toString());
    businessStore.setLogoUrl(DatabaseUtils::byteArrayToImageUrl(map.value("logo").toByteArray()));
    businessStore.setRackId(map.value("rack_id").toString());

    return businessStore;
}

QVariantMap BusinessStore::toVariantMap() const
{
    return {
        { "name", m_name },
        { "address", m_address },
        { "business_family", m_businessFamily },
        { "establishment_year", m_establishmentYear },
        { "phone_number", m_phoneNumber },
        { "logo", DatabaseUtils::imageUrlToByteArray(m_logoUrl) },
        { "rack_id", m_rackId }
    };
}

void BusinessStore::setId(int id)
{
    m_id = id;
}

void BusinessStore::setName(const QString &name)
{
    m_name = name;
}

void BusinessStore::setAddress(const QString &address)
{
    m_address = address;
}

void BusinessStore::setBusinessFamily(const QString &businessFamily)
{
    m_businessFamily = businessFamily;
}

void BusinessStore::setEstablishmentYear(int establishmentYear)
{
    m_establishmentYear = establishmentYear;
}

void BusinessStore::setPhoneNumber(const QString &phoneNumber)
{
    m_phoneNumber = phoneNumber;
}

void BusinessStore::setLogoUrl(const QUrl &logoUrl)
{
    m_logoUrl = logoUrl;
}

void BusinessStore::setRackId(const QString &rackId)
{
    m_rackId = rackId;
}
