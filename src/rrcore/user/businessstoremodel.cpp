#include "businessstoremodel.h"
#include "user/userprofile.h"
#include "user/businessadmin.h"
#include "user/businessstore.h"

BusinessStoreModel::BusinessStoreModel(QObject *parent) :
    QAbstractListModel(parent),
    m_currentIndex(-1)
{
    connect(UserProfile::instance().businessAdmin(), &BusinessAdmin::businessStoresChanged,
            this, &BusinessStoreModel::reset);
}

QVariant BusinessStoreModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case BusinessStoreId:
        return UserProfile::instance().businessAdmin()->businessStores().at(index.row()).id();
    case NameRole:
        return UserProfile::instance().businessAdmin()->businessStores().at(index.row()).name();
    case AddressRole:
        return UserProfile::instance().businessAdmin()->businessStores().at(index.row()).address();
    case BusinessFamilyRole:
        return UserProfile::instance().businessAdmin()->businessStores().at(index.row()).businessFamily();
    case EstablishmentYearRole:
        return UserProfile::instance().businessAdmin()->businessStores().at(index.row()).establishmentYear();
    case PhoneNumberRole:
        return UserProfile::instance().businessAdmin()->businessStores().at(index.row()).phoneNumber();
    case LogoUrlRole:
        return UserProfile::instance().businessAdmin()->businessStores().at(index.row()).logoUrl();
    }

    return QVariant();
}

QHash<int, QByteArray> BusinessStoreModel::roleNames() const
{
    return {
        { BusinessStoreId, "business_store_id" },
        { NameRole, "name" },
        { AddressRole, "address" },
        { BusinessFamilyRole, "business_family" },
        { EstablishmentYearRole, "establishment_year" },
        { PhoneNumberRole, "phone_number" },
        { LogoUrlRole, "logo_url" }
    };
}

int BusinessStoreModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return UserProfile::instance().businessAdmin()->businessStores().count();
}

BusinessStore BusinessStoreModel::currentBusinessStore() const
{
    if (m_currentIndex < -1
            || m_currentIndex >= UserProfile::instance().businessAdmin()->businessStores().count())
        return BusinessStore();

    return UserProfile::instance().businessAdmin()->businessStores().at(m_currentIndex);
}

int BusinessStoreModel::currentIndex() const
{
    return m_currentIndex;
}

void BusinessStoreModel::setCurrentIndex(int currentIndex)
{
    if (currentIndex < -1
            || currentIndex >= UserProfile::instance().businessAdmin()->businessStores().count()
            || m_currentIndex == currentIndex)
        return;

    m_currentIndex = currentIndex;
    emit currentIndexChanged();
}

void BusinessStoreModel::reset()
{
    beginResetModel();
    endResetModel();
}
