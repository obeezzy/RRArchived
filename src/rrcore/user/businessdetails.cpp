#include "businessdetails.h"
#include "businessstore.h"

BusinessDetails::BusinessDetails(QObject *parent) :
    QObject(parent),
    m_establishmentYear(1959)
{

}

QUrl BusinessDetails::logoUrl() const
{
    return m_logoUrl;
}

QString BusinessDetails::name() const
{
    return m_name;
}

void BusinessDetails::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged();
}

QString BusinessDetails::address() const
{
    return m_address;
}

QString BusinessDetails::businessFamily() const
{
    return m_businessFamily;
}

void BusinessDetails::setAddress(const QString &address)
{
    if (m_address == address)
        return;

    m_address = address;
    emit addressChanged();
}

void BusinessDetails::setBusinessFamily(const QString &businessFamily)
{
    if (m_businessFamily == businessFamily)
        return;

    m_businessFamily = businessFamily;
    emit businessFamilyChanged();
}

QString BusinessDetails::phoneNumber() const
{
    return m_phoneNumber;
}

void BusinessDetails::setPhoneNumber(const QString &phoneNumber)
{
    if (m_phoneNumber == phoneNumber)
        return;

    m_phoneNumber = phoneNumber;
    emit phoneNumberChanged();
}

int BusinessDetails::establishmentYear() const
{
    return m_establishmentYear;
}

void BusinessDetails::extractFromBusinessStore(const BusinessStore &businessStore)
{
    setName(businessStore.name());
    setAddress(businessStore.address());
    setPhoneNumber(businessStore.phoneNumber());
    setEstablishmentYear(businessStore.establishmentYear());
    setLogoUrl(businessStore.logoUrl());
}

void BusinessDetails::setLogoUrl(const QUrl &logoUrl)
{
    if (m_logoUrl == logoUrl)
        return;

    m_logoUrl = logoUrl;
    emit logoUrlChanged();
}

void BusinessDetails::setEstablishmentYear(int establishmentYear)
{
    if (m_establishmentYear == establishmentYear)
        return;

    m_establishmentYear = establishmentYear;
    emit establishmentYearChanged();
}
